import http.tcpserver;

using namespace http;

int main() {
  // Create the server on 0.0.0.0:8080
  TCPServer server("0.0.0.0", 8080);

  // Register routes from template files
  server.registerRoute("/", server.loadTemplate("templates/home.html"));
  server.registerRoute("/about", server.loadTemplate("templates/about.html"));
  server.registerRoute("/contact",
                       server.loadTemplate("templates/contact.html"));
  server.registerRoute("/api", server.loadTemplate("templates/api.html"));

  // Set static file root
  server.setStaticRoot("public");

  // Start the server
  server.startServer();

  return 0;
}
