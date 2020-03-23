//
// Created by jzb on 2020/3/23.
//

#include "ChatServerEndPoint.hpp"

static const std::string kURI = "/chat";

const std::string &ChatServerEndPoint::GetUri(void) {
    return kURI;
}

void ChatServerEndPoint::OnOpen(WebSocketSession &session) {
    printf("ChatServerEndPoint::OnOpen %d\n", sessions_.size());
}

void ChatServerEndPoint::OnMessage(WebSocketSession &session, const std::string &message) {
    for (auto s:sessions_) {
        if (s != session) {
            s.sendText(message);
        }
    }
}

void ChatServerEndPoint::OnClose(WebSocketSession &session) {
    printf("ChatServerEndPoint::OnClose %d\n", sessions_.size());
}