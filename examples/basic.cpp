#include <iostream>

#include "network/include/server.hpp"
#include "parsing/include/http_types.hpp"
#include "parsing/include/parser.hpp"

aether::HttpResponse handleMain(aether::HttpRequest& request) {
    if (request.method != aether::Method::GET) return aether::HttpResponse(405);
    return aether::HttpResponse(200, R"(<h1 style="color: blue; ">Hi!</h1>)");
}

int main() {
    aether::HttpServer server("127.0.0.1", 5000);
    server.router.addCallback("/", handleMain);
    server.router.setDefaultCallback([](aether::HttpRequest&) {
        return aether::HttpResponse(404, "<h1>This site doesn't exists!</h1>");
        });
    server.run();
}