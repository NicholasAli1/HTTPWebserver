#ifndef INCLUDED_HTTP_TCPSERVER_LINUX
#define INCLUDED_HTTP_TCPSERVER_LINUX

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>


namespace http {

    class TCPServer {
    public:
        TCPServer(std::string ip_address, int port);
        ~TCPServer();
        void registerRoute(const std::string &path, const std::string &response);
        void startServer();
        void setStaticRoot(const std::string &root);


    private:
        std::string m_ip_address;
        int m_port;
        int m_socket;
        int m_new_socket;
        long m_incomingMessage;
        struct sockaddr_in m_socketAddress;
        unsigned int m_socketAddress_len;
        std::string m_serverMessage;
        std::map<std::string, std::string> m_routes;
        std::string m_staticRoot;
        size_t m_maxThreads = 10; // max concurrent clients
        std::atomic<size_t> m_activeThreads {0};
        void closeServer();
        std::string buildResponse(const std::string &path);
        void handleClient(int clientSocket, struct sockaddr_in clientAddr);
        static std::string getContentType(const std::string &path); 
    };

}

#endif
