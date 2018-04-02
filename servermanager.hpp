//
// Created by tony on 18-3-26.
//

#ifndef LIMENET_SERVERMANAGER_HPP
#define LIMENET_SERVERMANAGER_HPP
#include <event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <memory>

class ServerManager{
public:
    static ServerManager& instance() {
        static ServerManager mngr;
        return mngr;
    }

    event_base* get_base() { return base_; }

    void start_all() { event_base_dispatch(base_); }
    void reinit() { event_reinit(base_); }
private:
    ServerManager() { base_ = event_base_new(); }

    event_base* base_;
};
#endif //LIMENET_SERVERMANAGER_HPP
