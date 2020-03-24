#include "http/HTTP.hpp"
#include "ChatServerEndPoint.hpp"
#include "ImageServerEndPoint.hpp"
#include <iostream>

int main() {
    HTTP &http = HTTP::getInstance()
            .AddEndPoint(ImageServerEndPoint::getInstance())
            .AddHandler("ImageService", "test", ImageServerEndPoint::handleTest)

            .AddEndPoint(ChatServerEndPoint::getInstance())
            .AddHandler("HelloService", "hello", [](RoutingContext &rc) {
                std::string data = "{\"key\":\"value\"}";
                rc.response
                        .header("X-SERVICE", "test")
                        .header("X-ACTION", "test")
                        .end(data);
            });
    HTTP_OPTION option;
    option.document_root = "/home/jzb/CLionProjects/http_test/www";
    http.Start(option);
    return 0;
}

