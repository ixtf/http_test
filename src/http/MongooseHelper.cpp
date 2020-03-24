//
// Created by jzb on 2020/3/23.
//

#include "MongooseHelper.hpp"

static const mg_str MethodGET = MG_MK_STR("GET");
static const mg_str MethodPOST = MG_MK_STR("POST");

bool MongooseHelper::StrEqual(const mg_str *s1, const mg_str *s2) {
    return s1->len == s2->len && memcmp(s1->p, s2->p, s2->len) == 0;
}

bool MongooseHelper::IsWebsocket(const mg_connection *nc) {
    return nc->flags & MG_F_IS_WEBSOCKET;
}

bool MongooseHelper::IsGET(const http_message *hm) {
    return StrEqual(&MethodGET, &hm->method);
}

bool MongooseHelper::IsPOST(const http_message *hm) {
    return StrEqual(&MethodPOST, &hm->method);
}

void MongooseHelper::End_health(mg_connection *nc) {
    mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
}

void MongooseHelper::End_404(mg_connection *nc) {
    mg_printf(nc, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
}

void MongooseHelper::End_500(mg_connection *nc) {
    mg_printf(nc, "HTTP/1.1 500 Server Error\r\nContent-Length: 0\r\n\r\n");
}

std::string MongooseHelper::ToString(const mg_connection *nc, const websocket_message *wm) {
    char buf[500];
    char addr[32];
    mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
    snprintf(buf, sizeof(buf), "%s %.*s", addr, (int) wm->size, wm->data);
    return std::string(buf, strlen(buf));
}
