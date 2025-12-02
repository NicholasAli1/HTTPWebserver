#include "http_tcpServer_linux.h"
#include <iostream>

int main() {
    // 1. Create the server on 0.0.0.0:8080
    http::TCPServer server("0.0.0.0", 8080);

    // 2. Register some routes
    server.registerRoute("/", "<h1>Home Page</h1>");
    server.registerRoute("/hello", "<h1>Hello Route</h1>");
    server.registerRoute("/form",
        "<form method='POST' action='/submit'>"
        "<input type='text' name='name' placeholder='Name'>"
        "<input type='submit'>"
        "</form>"
    );

    // 3. Set static file root (make sure this folder exists)
    server.setStaticRoot("../public");  // e.g., ../public/index.html

    // 4. Start the server
    server.startServer();

    return 0;
}
