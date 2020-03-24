//
// Created by jzb on 2020/3/23.
//

#include "ServerEndPoint.hpp"

void WebSocketSession::sendText(const std::string &content) {
    mg_send_websocket_frame(nc_, WEBSOCKET_OP_TEXT, content.c_str(), content.size());
}

void WebSocketSession::sendBinary(const void *buffer, const long size) {
    mg_send_websocket_frame(nc_, WEBSOCKET_OP_BINARY, buffer, size);
}

void ServerEndPoint::OnOpen(mg_connection *nc_) {
    WebSocketSession session(nc_);
    sessions_.push_back(session);
    OnOpen(session);
}

void ServerEndPoint::OnMessage(mg_connection *nc, websocket_message *wm) {
    for (auto session:sessions_) {
        if (session == nc) {
            OnMessage(session, MongooseHelper::ToString(nc, wm));
            break;
        }
    }
}

void ServerEndPoint::BroadcastText(const std::string &content) {
    for (auto session:sessions_) {
        session.sendText(content);
    }
}

void ServerEndPoint::BroadcastBinary(const void *buffer, const long size) {
    for (auto session:sessions_) {
        session.sendBinary(buffer, size);
    }
}

void ServerEndPoint::OnClose(mg_connection *nc_) {
    // fixme vector erase 再加强一下
    std::vector<WebSocketSession> sessions;
    for (auto session:sessions_) {
        if (session == nc_) {
            OnClose(session);
        } else {
            sessions.push_back(session);
        }
    }
    sessions_ = sessions;
}