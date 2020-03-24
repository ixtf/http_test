//
// Created by jzb on 2020/3/23.
//

#include "ChatServerEndPoint.hpp"

static const std::string kURI = "/chat";

const std::string &ChatServerEndPoint::GetUri(void) {
    return kURI;
}

void ChatServerEndPoint::OnMessage(WebSocketSession &session, const std::string &message) {
    for (auto s:sessions_) {
        if (s != session) {
            s.sendText(message);
        }
    }
}
