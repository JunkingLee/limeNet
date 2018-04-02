//
// Created by tony on 18-3-12.
//

#ifndef LIMENET_MUTILSERVER_HPP
#define LIMENET_MUTILSERVER_HPP
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <thread>
#include <fcntl.h>
#include <vector>
#include <atomic>
#include <arpa/inet.h>
#include "conn.hpp"
#include "connqueue.hpp"
#include "libeventThread.hpp"
#include "servermanager.hpp"

std::atomic<int> i(0);
struct Connection{
    short port;
    evutil_socket_t  confd;
    char*   ip;
};
class MultiServer{
public:
    MultiServer(int count){
        m_ThreadCount=count;
        m_Port=-1;
        m_MainBase=new LibeventThread;
        m_Threads=new LibeventThread[m_ThreadCount];
        //std::unique_ptr<LibeventThread> mainBase(new LibeventThread());
        //m_MainBase=std::move(mainBase);
        m_MainBase->tid=std::this_thread::get_id();
        //m_MainBase->base=event_base_new();
        m_MainBase->base=ServerManager::instance().get_base();
        memset(m_SignalEvents,0,sizeof(m_SignalEvents));

        for(int i=0;i<m_ThreadCount;i++){
            //std::unique_ptr<LibeventThread> ptr(new LibeventThread());
            //SetupThread(ptr.get());
            //m_Threads.push_back(std::move(ptr));
            SetupThread(&m_Threads[i]);
        }
    }

    virtual ~MultiServer(){
        StopRun(nullptr);
        //event_base_free(m_MainBase->base);
        for(int i=0;i<m_ThreadCount;i++){
            event_base_free(m_Threads[i].base);
        }
        delete m_MainBase;
        delete[] m_Threads;
    }
    void SetPort(int port){
        m_Port=port;
    }

    bool StartRun(){
        evconnlistener *listener;
        if(m_Port!=EXIT_CODE){
            sockaddr_in sin;
            memset(&sin,0,sizeof(sin));
            sin.sin_family=AF_INET;
            sin.sin_port=htons(m_Port);
            listener=evconnlistener_new_bind(m_MainBase->base,ListenerEventCb,(void*)this,
                LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,-1,(sockaddr*)&sin,sizeof(sockaddr_in));
            if(nullptr==listener) {
                fprintf(stderr, "listener error:%s\n", strerror(errno));
                exit(1);
            }
        }
        //std::thread th[m_ThreadCount];
        for(int i=0;i<m_ThreadCount;i++){
            m_vThreads.push_back(std::thread(WorkerLibevent,(void*)&m_Threads[i]));
        }
        AddSignalEvent(SIGINT, nullptr);
        //event_base_dispatch(m_MainBase->base);
/*
        if(m_Port!=EXIT_CODE){
            evconnlistener_free(listener);
        }*/
    }
    void StopRun(timeval* tv){
        int contant=EXIT_CODE;
        for(int i=0;i<m_ThreadCount;i++){
            write(m_Threads[i].notifySendFd,&contant,sizeof(int));
        }
        event_base_loopexit(m_MainBase->base,tv);
    }

    bool AddSignalEvent(int sig,void(*ptr)(int,short,void*)){
        if(sig>=MAX_SIGNAL)
            return false;

        event	*ev=evsignal_new(m_MainBase->base,sig,ptr,(void*)this);
        if(!ev||event_add(ev,NULL)<0)
        {
            event_del(ev);
            return false;
        }

        if(NULL!=m_SignalEvents[sig])
            DeleteSignalEvent(sig);
        m_SignalEvents[sig]=ev;

        return true;
    }
    bool DeleteSignalEvent(int sig){
        event *ev=m_SignalEvents[sig];
        if(sig>=MAX_SIGNAL||NULL==ev)
            return false;
        event_del(ev);
        ev=NULL;
        return true;
    }
    event* AddTimerEvent(void(*ptr)(int,short,void*),timeval tv,bool once){
        int flag=0;
        if(!once)
            flag=EV_PERSIST;

        event *ev=new event;
        event_assign(ev,m_MainBase->base,-1,flag,ptr,(void*)this);
        if(event_add(ev,&tv)<0)
        {
            event_del(ev);
            return NULL;
        }
        return ev;
    }
    bool DeleteTimerEvent(event *ev){
        int res=event_del(ev);
        return (0==res);
    }
private:
    static const int EXIT_CODE=-1;
    static const int MAX_SIGNAL=256;
private:
    int m_ThreadCount;
    int m_Port;
    LibeventThread *m_MainBase; //主线程的libevent事件处理机
    //std::unique_ptr<LibeventThread> m_MainBase;
    //std::unique_ptr<LibeventThread[]> m_Threads;
    //std::vector<std::unique_ptr<LibeventThread>> m_Threads;
    LibeventThread *m_Threads;  //存储各个子线程信息的数组
    event *m_SignalEvents[MAX_SIGNAL];      //自定义的信号处理
    std::vector<std::thread> m_vThreads;
private:
    void SetupThread(LibeventThread *me){
        int res;
        me->tcpConnect=this;
        me->base=event_base_new();
        int fds[2];
        res=pipe(fds);
        me->notifyReceiveFd=fds[0];
        me->notifySendFd=fds[1];
        event_set(&me->notifyEvent,me->notifyReceiveFd,EV_READ|EV_PERSIST,ThreadProcess,me);
        event_base_set(me->base,&me->notifyEvent);
        res=event_add(&me->notifyEvent,0);

    }
    static void *WorkerLibevent(void *arg){
        LibeventThread *me=(LibeventThread*)arg;
        event_base_dispatch(me->base);
    }
    static void ThreadProcess(int fd,short which,void* arg){
        LibeventThread *me=(LibeventThread*)arg;
        int pipefd=me->notifyReceiveFd;
        evutil_socket_t confd;
        struct Connection connection;
        //read(pipefd,&confd,sizeof(evutil_socket_t));
        read(pipefd,&connection,sizeof(struct Connection));
        confd=connection.confd;
        if(EXIT_CODE==confd){
            event_base_loopbreak(me->base);
            return;
        }
        struct bufferevent *bev;
        bev=bufferevent_socket_new(me->base,confd,BEV_OPT_CLOSE_ON_FREE);
        if(!bev){
            event_base_loopbreak(me->base);
            return;
        }

        Conn* conn=me->connectQueue.InsertConn<CIEC104Ter>(confd,connection.ip,connection.port,me);
        printf("new conn %d,ip=%s port=%d\n",confd,conn->ip,conn->port);
        bufferevent_setcb(bev,ReadEventCb,WriteEventCb,CloseEventCb,conn);
        bufferevent_enable(bev,EV_WRITE);
        bufferevent_enable(bev,EV_READ);
        me->tcpConnect->ConnectionEvent(conn);
    }

    static void ListenerEventCb(evconnlistener *listener,evutil_socket_t fd,sockaddr *sa,int socklen,void *user_data){
        MultiServer *server=(MultiServer*)user_data;
        struct sockaddr_in * adds=(struct sockaddr_in *)sa;
//        char* ip = NULL;
//        ip = inet_ntoa(adds->sin_addr);
//        short port=adds->sin_port;

        int num=rand()%server->m_ThreadCount;
        int sendfd=server->m_Threads[num].notifySendFd;
        struct Connection connection;
        connection.confd=fd;
        connection.ip=inet_ntoa(adds->sin_addr);
        connection.port=adds->sin_port;
        //write(sendfd,&fd,sizeof(evutil_socket_t));
        write(sendfd,&connection,sizeof(struct Connection));
    }

    static void ReadEventCb(struct bufferevent *bev,void* data){
        Conn* conn=(Conn*)data;
        conn->m_ReadBuf=bufferevent_get_input(bev);
        conn->m_WriteBuf=bufferevent_get_output(bev);
        conn->m_Thread->tcpConnect->ReadEvent(conn);
    }

    static void WriteEventCb(struct bufferevent *bev,void *data){
        Conn* conn=(Conn*)data;
        conn->m_ReadBuf=bufferevent_get_input(bev);
        conn->m_WriteBuf=bufferevent_get_output(bev);
        conn->m_Thread->tcpConnect->WriteEvent(conn);
    }

    static void CloseEventCb(struct bufferevent *bev,short events,void *data){
        Conn	*conn=(Conn*)data;
        conn->m_Thread->tcpConnect->CloseEvent(conn,events);
        conn->GetThread()->connectQueue.DeleteConn(conn);
        bufferevent_free(bev);
    }

protected:
    virtual void ConnectionEvent(Conn *conn){}
    virtual void ReadEvent(Conn* conn){}
    virtual void WriteEvent(Conn* conn){}
    virtual void CloseEvent(Conn* conn,short events){}
};


#endif //LIMENET_MUTILSERVER_HPP
