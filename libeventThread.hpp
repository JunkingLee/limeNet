//
// Created by tony on 18-3-12.
//

#ifndef LIMENET_LIBEVENTTHREAD_HPP
#define LIMENET_LIBEVENTTHREAD_HPP
#include <thread>
#include "connqueue.hpp"

struct LibeventThread{
    std::thread::id tid;
    struct event_base *base;    //libevent的事件处理机
    struct event notifyEvent;   //监听管理的事件机
    int notifyReceiveFd;    //管道的接收端
    int notifySendFd;       //管道的发送端
    ConnQueue connectQueue; //socket连接的链表
    MultiServer *tcpConnect;
};
#endif //LIMENET_LIBEVENTTHREAD_HPP
