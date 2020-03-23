//
// Created by jzb on 2020/3/23.
//

#include "ImageServerEndPoint.hpp"

static const std::string kURI = "/image";

const std::string &ImageServerEndPoint::GetUri(void) {
    return kURI;
}

void ImageServerEndPoint::OnOpen(WebSocketSession &session) {
    printf("ImageServerEndPoint::OnOpen %d\n", sessions_.size());
}

void ImageServerEndPoint::OnMessage(WebSocketSession &session, const std::string &message) {
    for (auto s:sessions_) {
        if (s != session) {
            s.sendText(message);
        }
    }
}

void ImageServerEndPoint::OnClose(WebSocketSession &session) {
    printf("ImageServerEndPoint::OnClose %d\n", sessions_.size());
}
