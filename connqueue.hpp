//
// Created by tony on 18-3-12.
//

#ifndef LIMENET_CONNQUEUE_HPP
#define LIMENET_CONNQUEUE_HPP


#include "conn.hpp"
#include "iec104Ter.hpp"


class ConnQueue{
private:
    Conn    *m_head;
    //std::shared_ptr<Conn> m_head;
    //std::shared_ptr<Conn> m_tail;
    Conn    *m_tail;
public:
    ConnQueue(){

        m_head=new CIEC104Ter(0);
        m_tail=new CIEC104Ter(0);
        m_head->m_Prev=m_tail->m_Next=NULL;
        m_head->m_Next=m_tail;
        m_tail->m_Prev=m_head;

//        std::shared_ptr<Conn> head=std::make_shared<CIEC104Ter>(0);
//        std::shared_ptr<Conn> tail=std::make_shared<CIEC104Ter>(0);
//        m_head=std::move(head);
//        m_tail=std::move(tail);
//        m_head->m_Next=m_tail;

    }
    ~ConnQueue(){
        Conn *tcur,*tnext;
        tcur=m_head;

        while(tcur!=NULL)
        {
            tnext=tcur->m_Next;
            delete tcur;
            tcur=tnext;
        }
    }
    template <typename T>
    Conn* InsertConn(int fd,char* ip,short port,LibeventThread *t){
        Conn *c=new T(fd);
        c->m_Thread=t;
        c->ip=ip;
        c->port=port;
        Conn *next=m_head->m_Next;

        c->m_Prev=m_head;
        c->m_Next=m_head->m_Next;
        m_head->m_Next=c;
        next->m_Prev=c;
        return c;

//        std::shared_ptr<Conn> c=std::make_shared<T>(fd);
//        c->m_Thread=t;
//        std::weak_ptr<Conn> next=m_head->m_Next;
//        c->m_Prev=m_head;
//        c->m_Next=m_head->m_Next;
//        m_head->m_Next=c;
//        next.lock()->m_Prev=c;
//        return m_head->m_Next;
    }
    void DeleteConn(Conn* c){

//        c->m_Prev.lock()->m_Next=std::move(c->m_Next);
//        c->m_Next.lock()->m_Prev=c->m_Prev;

        c->m_Prev->m_Next=c->m_Next;
        c->m_Next->m_Prev=c->m_Prev;
        delete c;
    }
    void Show(){

    }
};
#endif //LIMENET_CONNQUEUE_HPP
