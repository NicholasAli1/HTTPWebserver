cmake_minimum_required(VERSION 4.0)
project(HTTPWebserver)

set(CMAKE_CXX_STANDARD 20)

add_executable(HTTPWebserver server_linux.cpp
        http_tcpServer_linux.h
        http_tcpServer_linux.cpp)
