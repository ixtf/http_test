#include "http/HTTP.hpp"
#include "http/RoutingContext.hpp"
#include <iostream>
#include <algorithm>

std::string bodyHandler(std::string &body) {
    return "";
}

void test(RoutingContext &rc) {
    std::string data = "{\"key\":\"value\"}";
//    rc.request.body([](std::string &body) -> int {
////    });
//    std::string s = rc.request.body(bodyHandler);
    rc.response
            .header("X-SERVICE", "test")
            .header("X-ACTION", "test")
            .end(data);
}

int main() {
    HTTP &http = HTTP::getInstance();
    int i = 123;
//    http.AddHandler("test", "test", test);
    http.AddHandler("test", "test", [](RoutingContext &rc) {
        std::string data = "{\"key\":\"value\"}";
//        i++;
//        std::cout << i << std::endl;
        rc.response
                .header("X-SERVICE", "test")
                .header("X-ACTION", "test")
                .end(data);
    });
    HTTP_OPTION option;
    option.port = "8080";
    http.Start(option);

    std::cout << "Hello, World!" << std::endl;

    return 0;
}

