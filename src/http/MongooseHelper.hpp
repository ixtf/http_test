//
// Created by jzb on 2020/3/23.
//

#ifndef HTTP_TEST_MONGOOSEHELPER_HPP
#define HTTP_TEST_MONGOOSEHELPER_HPP

#include "mongoose.h"
#include <string>

class MongooseHelper {
public:
    static bool StrEqual(const mg_str *s1, const mg_str *s2);

    static bool IsGET(const http_message *hm);

    static bool IsPOST(const http_message *hm);

    static bool IsWebsocket(const mg_connection *nc);

    static void End_health(mg_connection *nc);

    static void End_404(mg_connection *nc);

    static void End_500(mg_connection *nc);

    static std::string ToString(const mg_connection *nc, const websocket_message *wm);

private:
    MongooseHelper() {};
};


#endif //HTTP_TEST_MONGOOSEHELPER_HPP
