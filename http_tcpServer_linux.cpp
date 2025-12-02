#include "http_tcpServer_linux.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <atomic>
#include <map>
#include <chrono>
#include <arpa/inet.h>

namespace http {

// ------------------- Thread limit -------------------
std::atomic<size_t> m_activeThreads {0};
size_t m_maxThreads = 10;

// ------------------- Constructor -------------------
TCPServer::TCPServer(std::string ip_address, int port)
    : m_ip_address(ip_address), m_port(port)
{
    m_socketAddress_len = sizeof(m_socketAddress);

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port = htons(m_port);
    m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

    if (bind(m_socket, (struct sockaddr *)&m_socketAddress, m_socketAddress_len) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(m_socket, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }
}

// ------------------- Destructor -------------------
TCPServer::~TCPServer() {
    closeServer();
}

// ------------------- Start server -------------------
void TCPServer::startServer() {
    std::cout << "Server running on " << m_ip_address << ":" << m_port << "\n";

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(m_socket, (struct sockaddr *)&clientAddr, &clientLen);

        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        // Wait if too many threads
        while (m_activeThreads >= m_maxThreads) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        m_activeThreads++;
        std::thread t([this, clientSocket, clientAddr]() {
            handleClient(clientSocket, clientAddr);
            m_activeThreads--;
        });
        t.detach();
    }
}

// ------------------- Close server -------------------
void TCPServer::closeServer() {
    close(m_socket);
}

// ------------------- Content type helper -------------------
std::string TCPServer::buildResponse(const std::string &path) {
    std::string response;
    std::string body;
    std::string status;
    std::string contentType = "text/html"; // Default

    // 1. Check registered routes first
    if (m_routes.find(path) != m_routes.end()) {
        body = m_routes[path];
        status = "200 OK";
    }
    // 2. Check static files
    else if (!m_staticRoot.empty()) {
        std::string filePath = m_staticRoot + path;
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (file) {
            std::ostringstream ss;
            ss << file.rdbuf();
            body = ss.str();
            status = "200 OK";
            contentType = getContentType(path); // Use the new method here
        } else {
            body = "<h1>404 Not Found</h1>";
            status = "404 Not Found";
        }
    }
    // 3. Not found
    else {
        body = "<h1>404 Not Found</h1>";
        status = "404 Not Found";
    }

    response =
        "HTTP/1.1 " + status + "\r\n"
        "Content-Type: " + contentType + "\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" +
        body;

    return response;
}

std::string TCPServer::getContentType(const std::string &path) {
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".css") != std::string::npos) return "text/css";
    if (path.find(".js") != std::string::npos) return "text/javascript";
    if (path.find(".png") != std::string::npos) return "image/png";
    if (path.find(".jpg") != std::string::npos) return "image/jpeg";
    return "text/plain";
}

// ------------------- URL decode -------------------
std::string urlDecode(const std::string &str) {
    std::string decoded;
    char hex[3] = {0};
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%') {
            hex[0] = str[i + 1];
            hex[1] = str[i + 2];
            decoded += static_cast<char>(std::strtol(hex, nullptr, 16));
            i += 2;
        } else if (str[i] == '+') {
            decoded += ' ';
        } else {
            decoded += str[i];
        }
    }
    return decoded;
}

// ------------------- Register route -------------------
void TCPServer::registerRoute(const std::string &path, const std::string &response) {
    m_routes[path] = response;
}

// ------------------- Set static root -------------------
void TCPServer::setStaticRoot(const std::string &root) {
    m_staticRoot = root;
}

// ------------------- Handle client -------------------
void TCPServer::handleClient(int clientSocket, struct sockaddr_in clientAddr) {
    char buffer[30000] = {0};
    long bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        close(clientSocket);
        return;
    }

    std::string request(buffer);
    std::istringstream ss(request);
    std::string method, path, httpVersion;
    ss >> method >> path >> httpVersion;

    // Log client
    std::string clientIP = inet_ntoa(clientAddr.sin_addr);
    std::cout << "[" << clientIP << "] " << method << " " << path << std::endl;

    // POST data
    std::map<std::string, std::string> postData;
    if (method == "POST") {
        size_t headerEnd = request.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            std::string body = request.substr(headerEnd + 4);

            std::istringstream bodyStream(body);
            std::string pair;
            while (std::getline(bodyStream, pair, '&')) {
                size_t eq = pair.find('=');
                if (eq != std::string::npos) {
                    std::string key = urlDecode(pair.substr(0, eq));
                    std::string value = urlDecode(pair.substr(eq + 1));
                    postData[key] = value;
                }
            }

            // Optional: print POST data
            for (auto &kv : postData) {
                std::cout << kv.first << " = " << kv.second << std::endl;
            }
        }
    }

    std::string response = buildResponse(path);
    write(clientSocket, response.c_str(), response.size());

    close(clientSocket);
}

} // namespace http
