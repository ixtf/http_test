//
// Created by jzb on 2020/3/23.
//

#ifndef HTTP_TEST_SERVERENDPOINT_HPP
#define HTTP_TEST_SERVERENDPOINT_HPP

#include "MongooseHelper.hpp"
#include <vector>
#include <string>

class WebSocketSession {
public:
    WebSocketSession(mg_connection *nc) : nc_(nc) {};

    bool operator==(const WebSocketSession &s) const {
        return nc_ == s.nc_;
    }

    bool operator!=(const WebSocketSession &s) const {
        return nc_ != s.nc_;
    }

    bool operator==(const mg_connection *s) const {
        return nc_ == s;
    }

    bool operator!=(const mg_connection *s) const {
        return nc_ != s;
    }

    void sendText(const std::string &content);

private:
    mg_connection *nc_;
};

class ServerEndPoint {
public:
    virtual const std::string &GetUri(void) = 0;

    void OnOpen(mg_connection *nc_);

    void OnMessage(mg_connection *nc_, websocket_message *wm);

    void BroadcastText(const std::string &content);

    void BroadcastBinary(const char *buffer);

    void OnClose(mg_connection *nc_);
    // todo 了解cpp异常
//    virtual void OnError_(Exception err);
protected:
    std::vector<WebSocketSession> sessions_;

    virtual void OnOpen(WebSocketSession &session) = 0;

    virtual void OnMessage(WebSocketSession &session, const std::string &message) = 0;

    virtual void OnClose(WebSocketSession &session) = 0;
};

#endif //HTTP_TEST_SERVERENDPOINT_HPP
