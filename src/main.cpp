#include "http/HTTP.hpp"
#include "ChatServerEndPoint.hpp"
#include "ImageServerEndPoint.hpp"
#include <iostream>

int main() {
    HTTP &http = HTTP::getInstance()
            .AddEndPoint(ChatServerEndPoint::getInstance())
            .AddEndPoint(ImageServerEndPoint::getInstance())

            .AddHandler("HelloService", "hello", [](RoutingContext &rc) {
                std::string data = "{\"key\":\"value\"}";
                rc.response
                        .header("X-SERVICE", "test")
                        .header("X-ACTION", "test")
                        .end(data);
            })
            .AddHandler("ImageService", "test", [](RoutingContext &rc) {
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
                ImageServerEndPoint::getInstance().BroadcastBinary(buffer, lSize);

                fclose(pFile);
                free(buffer);
                std::string data = "{\"status\":\"ok\"}";
//                rc.response.header().end();
            });
    HTTP_OPTION option;
    option.document_root = "/home/jzb/CLionProjects/http_test/www";
    http.Start(option);
    return 0;
}

