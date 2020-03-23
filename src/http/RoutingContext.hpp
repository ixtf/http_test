//
// Created by ixtf on 2020/3/22.
//

#ifndef HTTP_TEST_ROUTINGCONTEXT_HPP
#define HTTP_TEST_ROUTINGCONTEXT_HPP

#include "mongoose.h"
#include <string>
#include <map>
#include <iostream>
#include <functional>

inline static bool mg_str_equal(const mg_str *s1, const mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

const std::string CONTENT_TYPE = "Content-Type";

class RoutingRequest {
public:
    RoutingRequest(mg_connection *nc, http_message *hm) : nc_(nc), hm_(hm) {}

    ~RoutingRequest() {
        std::cout << "~RoutingRequest()" << std::endl;
    }

    std::string header(const std::string &key);

    template<typename T>
    T body(std::function<T(std::string &body)> fun) {
        std::string body(hm_->body.p, hm_->body.len);
        return fun(body);
    }

private:
    mg_connection *nc_;
    http_message *hm_;
};

class RoutingResponse {
public:
    RoutingResponse(mg_connection *nc) : nc_(nc), status_(200) {
        headers_[CONTENT_TYPE] = "application/json";
    }

    ~RoutingResponse() {
        std::cout << "~RoutingResponse()" << std::endl;
    }

    inline RoutingResponse &header(const std::string &key, const std::string &value) {
        headers_[key] = value;
        return *this;
    }

    void end();

    void end(const std::string &body) {
        body_ = body;
        end();
    }

    void end(const int status, const std::string &body) {
        status_ = status;
        end(body);
    }

public:
    inline static void End_health(mg_connection *nc) {
        mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    }

    inline static void End_404(mg_connection *nc) {
        mg_printf(nc, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
    }

    inline static void End_500(mg_connection *nc) {
        mg_printf(nc, "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n");
    }

private:
    mg_connection *nc_;
    std::map<std::string, std::string> headers_;
    int status_;
    std::string body_;
};

class RoutingContext {
public:
    RoutingContext(mg_connection *nc, http_message *hm) : request(nc, hm), response(nc) {}

    ~RoutingContext() {
        std::cout << "~RoutingContext()" << std::endl;
    }

    RoutingRequest request;
    RoutingResponse response;
};

#endif //HTTP_TEST_ROUTINGCONTEXT_HPP
