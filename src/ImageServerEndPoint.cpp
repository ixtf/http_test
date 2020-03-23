//
// Created by jzb on 2020/3/23.
//

#include "ImageServerEndPoint.hpp"

static const std::string kURI = "/image";

const std::string &ImageServerEndPoint::GetUri(void) {
    return kURI;
}

void ImageServerEndPoint::OnMessage(WebSocketSession &session, const std::string &message) {
    for (auto s:sessions_) {
        if (s != session) {
            s.sendText(message);
        }
    }
}
