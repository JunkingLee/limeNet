//
// Created by tony on 18-3-12.
//

#ifndef LIMENET_CONN_HPP
#define LIMENET_CONN_HPP

#include <event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <memory>


class MultiServer;
struct LibeventThread;
class Conn{
    friend class ConnQueue;
    friend class MultiServer;
public:
    Conn(int fd=0):m_fd(fd){
        m_Prev= nullptr;
        m_Next= nullptr;
    }
    ~Conn(){

    }
protected:
    const int m_fd;
    evbuffer    *m_ReadBuf;
    evbuffer    *m_WriteBuf;
    Conn        *m_Prev;
    //std::weak_ptr<Conn> m_Prev;
    Conn        *m_Next;
    //std::weak_ptr<Conn> m_Next;
    LibeventThread  *m_Thread;
    char*   ip;
    short   port;
public:
    LibeventThread  *GetThread(){return m_Thread;}
    int GetFd(){return m_fd;}
    char* GetIP(){return ip;}
    short GetPort(){ return  port;}
    //获取可读数据的长度
    int GetReadBufferLen(){
        return evbuffer_get_length(m_ReadBuf);
    }
    //从读缓冲区中取出len个字节的数据，存入buffer中，若不够，则读出所有数据
    int GetReadBuffer(char* buffer,int len){
        return evbuffer_remove(m_ReadBuf,buffer,len);
    }
    //从读缓冲区中复制出len个字节的数据，存入buffer中，若不够，则复制出所有数据
    //返回复制出数据的字节数
    //执行该操作后，数据还会留在缓冲区中，buffer中的数据只是原数据的副本
    int CopyReadBuffer(char* buffer,int len){
        return evbuffer_copyout(m_ReadBuf,buffer,len);
    }
    //获取可写数据的长度
    int GetWriteBufferLen(){
        return evbuffer_get_length(m_WriteBuf);
    }
    //将数据加入写缓冲区，准备发送
    int AddToWriteBuffer(char* buffer,int len){
        return evbuffer_add(m_WriteBuf,buffer,len);
    }
    //将读缓冲区中的数据移动到写缓冲区
    void MoveBufferData(){
        evbuffer_add_buffer(m_WriteBuf,m_ReadBuf);
    }
public:
    virtual void OnMsg()=0;
    virtual void ReadMsg()=0;
    virtual void SendMsg()=0;
};
#endif //LIMENET_CONN_HPP
