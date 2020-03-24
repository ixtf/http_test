//
// Created by jzb on 2020/3/23.
//

#ifndef HTTP_TEST_IMAGESERVERENDPOINT_HPP
#define HTTP_TEST_IMAGESERVERENDPOINT_HPP

#include "http/HTTP.hpp"

class ImageServerEndPoint : public ServerEndPoint {
public:
    ImageServerEndPoint(ImageServerEndPoint const &) = delete;

    void operator=(ImageServerEndPoint const &) = delete;

    static ImageServerEndPoint &getInstance() {
        static ImageServerEndPoint instance;
        return instance;
    }

    const std::string &GetUri() override;

    static void handleTest(RoutingContext &rc);

protected:
    void OnOpen(WebSocketSession &session) override {};

    void OnMessage(WebSocketSession &session, const std::string &message) override;

    void OnClose(WebSocketSession &session) override {};

private:
    ImageServerEndPoint() {};
};


#endif //HTTP_TEST_IMAGESERVERENDPOINT_HPP
