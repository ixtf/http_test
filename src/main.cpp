#include "http/HTTP.hpp"
#include "http/RoutingContext.hpp"
#include <iostream>

int main() {
    HTTP &http = HTTP::getInstance()
            .AddHandler("HelloService", "hello", [](RoutingContext &rc) {
                std::string data = "{\"key\":\"value\"}";
                rc.response
                        .header("X-SERVICE", "test")
                        .header("X-ACTION", "test")
                        .end(data);
            });
    http.Start();
    return 0;
}

