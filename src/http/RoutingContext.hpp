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

const std::string CONTENT_TYPE = "Content-Type";

class RoutingRequest {
public:
    RoutingRequest(mg_connection *nc, http_message *hm) : nc_(nc), hm_(hm) {}

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

private:
    mg_connection *nc_;
    std::map<std::string, std::string> headers_;
    int status_;
    std::string body_;
};

class RoutingContext {
public:
    RoutingContext(mg_connection *nc, http_message *hm) : request(nc, hm), response(nc) {}

    RoutingRequest request;
    RoutingResponse response;
};

#endif //HTTP_TEST_ROUTINGCONTEXT_HPP
