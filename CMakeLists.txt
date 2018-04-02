cmake_minimum_required(VERSION 3.8)
project(limeNet)

#AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp msgformat.hpp exmsg.hpp conn.hpp connqueue.hpp mutilserver.hpp libeventThread.hpp tdccpmsg.hpp iec104Ter.hpp iec104msg.hpp testserver.hpp tdccpTer.hpp ThreadLock.hpp servermanager.hpp)

add_executable(limeNet ${SOURCE_FILES})
target_link_libraries(limeNet event)