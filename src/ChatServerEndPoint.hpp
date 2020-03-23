//
// Created by jzb on 2020/3/23.
//

#ifndef HTTP_TEST_CHATSERVERENDPOINT_HPP
#define HTTP_TEST_CHATSERVERENDPOINT_HPP

#include "http/ServerEndPoint.hpp"

class ChatServerEndPoint : public ServerEndPoint {
public:
    ChatServerEndPoint(ChatServerEndPoint const &) = delete;

    void operator=(ChatServerEndPoint const &) = delete;

    static ChatServerEndPoint &getInstance() {
        static ChatServerEndPoint instance;
        return instance;
    }

    const std::string &GetUri() override;

protected:
    void OnOpen(WebSocketSession &session) override;

    void OnMessage(WebSocketSession &session, const std::string &message) override;

    void OnClose(WebSocketSession &session) override;

private:
    ChatServerEndPoint() {};
};


#endif //HTTP_TEST_CHATSERVERENDPOINT_HPP
