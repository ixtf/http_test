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

void ImageServerEndPoint::handleTest(RoutingContext &rc) {
    FILE *pFile = fopen("/home/jzb/Pictures/desktop.jpg", "rb");
    if (pFile == NULL) {
        fputs("File error", stderr);
        exit(1);
    }

    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);

    char *buffer = (char *) malloc(sizeof(char) * lSize);
    if (buffer == NULL) {
        fputs("Memory error", stderr);
        exit(2);
    }

    size_t result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) {
        fputs("Reading error", stderr);
        exit(3);
    }
    getInstance().BroadcastBinary(buffer, lSize);

    fclose(pFile);
    free(buffer);
    std::string data = "{\"status\":\"ok\"}";
    rc.response.end(data);
}
