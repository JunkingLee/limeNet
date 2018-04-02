#include <iostream>
#include "connqueue.hpp"
#include "testserver.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    //std::unique_ptr<ConnQueue<Conn>> connQueue(new ConnQueue<CIEC104Ter>());
//    ConnQueue connQueue;
//    Conn* conn1=connQueue.InsertConn<CIEC104Ter>(1, nullptr);
//    //Conn* conn2=connQueue.InsertConn<CIEC104Ter>(2, nullptr);
//    connQueue.Show();
    TestServer server(1);
    server.SetPort(2404);
    server.StartRun();

    TestServer server2(1);
    server2.SetPort(2405);
    server2.StartRun();

    ServerManager::instance().start_all();
    return 0;
}