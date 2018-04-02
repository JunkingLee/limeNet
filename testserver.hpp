//
// Created by tony on 18-3-13.
//

#ifndef LIMENET_TESTSERVER_HPP
#define LIMENET_TESTSERVER_HPP

#include "mutilserver.hpp"
#include "ThreadLock.hpp"

class TestServer: public MultiServer{
public:
    TestServer(int count) : MultiServer(count) {}
    virtual ~TestServer(){}
    static void QuitCb(int sig,short events,void* data){
        TestServer *me=(TestServer*)data;
        timeval tv={1,0};
        me->StopRun(&tv);
    }
    static void TimeOutCb(int id,short events,void* data){
        TestServer	*me=(TestServer*)data;
        char temp[33]="hello world\n";
        for(int i=0;i<me->vec.size();i++)
        {
            me->vec[i]->AddToWriteBuffer(temp,strlen(temp));
        }
    }
protected:
    void ReadEvent(Conn* conn){
        conn->ReadMsg();
    }
    void WriteEvent(Conn* conn){
        //char temp[33]="hello world";
        //conn->AddToWriteBuffer(temp,strlen(temp));
    }
    void ConnectionEvent(Conn* conn){
        TestServer *me=(TestServer*)conn->GetThread()->tcpConnect;
        me->vec.push_back(conn);
    }
    void CloseEvent(Conn* conn,short events){
        //printf("connection closed:%d\n",conn->GetFd());
        int confd=conn->GetFd();
        printf("connection closed %d,ip=%s port=%d\n",confd,conn->GetIP(),conn->GetPort());
    }
private:
    std::vector<Conn*> vec;
};
#endif //LIMENET_TESTSERVER_HPP
