//
// Created by jzb on 2020/3/23.
//

#include <iostream>
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

void ImageServerEndPoint::handleTest(RoutingContext &rc) {
    FILE *pFile = fopen("/home/jzb/Pictures/desktop.jpg", "rb");
    if (pFile == NULL) {
        std::string data = "{\"status\":\"File error\"}";
        rc.response.end(400, data);
        return;
    }

    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    char *buffer = (char *) malloc(sizeof(char) * lSize);
    if (buffer == NULL) {
        std::string data = "{\"status\":\"Memory error\"}";
        rc.response.end(400, data);
        return;
    }

    size_t result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {
        std::string data = "{\"status\":\"Reading error\"}";
        rc.response.end(400, data);
        free(buffer);
        return;
    }
    getInstance().BroadcastBinary(buffer, lSize);
    free(buffer);
    fclose(pFile);

    std::string data = "{\"status\":\"ok\"}";
    rc.response.end(data);
    std::cout << data << std::endl;
}
