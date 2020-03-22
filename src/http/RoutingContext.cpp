//
// Created by ixtf on 2020/3/22.
//

#include "RoutingContext.hpp"

void RoutingResponse::end() {
    std::string res = "HTTP/1.1 " + std::to_string(status_);
    switch (status_) {
        case 200: {
            res += " OK\r\n";
            std::string h = "Content-Length:" + std::to_string(body_.size()) + "\r\n";
            res += h;
            for (auto iter:headers_) {
                h = iter.first + ":" + iter.second + "\r\n";
                res += h;
            }
            res += "\r\n";
            if (body_.size() > 0) {
                res += body_;
            }
            mg_printf(nc_, "%s", res.c_str());
            return;
        }
        case 400: {
            res += " Bad Request\r\nContent-Length: 0\r\n\r\n";
            mg_printf(nc_, "%s", res.c_str());
            return;
        }
    }
}

//void RoutingContext::end(std::string body) {
//    mg_printf(nc_,
//              "HTTP/1.1 200 OK\r\n"
//              "Content-Type: application/json\r\n"
//              "Content-Length: %d\r\n\r\n%s",
//              (int) body.size(), body.c_str());
//}