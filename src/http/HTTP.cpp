//
// Created by ixtf on 2020/3/22.
//

#include "HTTP.hpp"
#include "MongooseHelper.hpp"
#include <iostream>

static bool kInit = true;
static mg_str kHealthUri = MG_MK_STR("/health");
static mg_str kApiUri = MG_MK_STR("/api");
static mg_str kService = MG_MK_STR("X-SERVICE");
static mg_str kAction = MG_MK_STR("X-ACTION");
static std::map<std::string, std::function<void(RoutingContext &)>> kHandlerMap;
static std::map<std::string, ServerEndPoint *> kEndPointMap;

static int kSigNum = 0;
static mg_mgr kMgMgr;
static mg_serve_http_opts kMgServeHttpOpts;

inline static void signalHandler(const int sig_num) {
    signal(sig_num, signalHandler);
    kSigNum = sig_num;
}

static std::function<void(RoutingContext &)> findHandler(http_message *hm) {
    std::string service;
    std::string action;
    for (int i = 0; i < MG_MAX_HTTP_HEADERS && hm->header_names[i].len > 0; i++) {
        mg_str hn = hm->header_names[i];
        mg_str hv = hm->header_values[i];
        if (MongooseHelper::StrEqual(&kService, &hn)) {
            service = std::string(hv.p, hv.len);
            continue;
        }
        if (MongooseHelper::StrEqual(&kAction, &hn)) {
            action = std::string(hv.p, hv.len);
            continue;
        }
    }
    if (service.size() > 0 && action.size() > 0) {
        auto iter = kHandlerMap.find(service + "." + action);
        return iter != kHandlerMap.end() ? iter->second : NULL;
    }
    return NULL;
}

static void mg_ev_handler(mg_connection *nc, int ev, void *ev_data) {
    switch (ev) {
        case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
            auto *hm = (http_message *) ev_data;
            auto entry = kEndPointMap.find(std::string(hm->uri.p, hm->uri.len));
            if (entry != kEndPointMap.end()) {
                entry->second->OnOpen(nc);
            }
            return;
        }
        case MG_EV_WEBSOCKET_FRAME: {
            auto *wm = (websocket_message *) ev_data;
            for (auto entry:kEndPointMap) {
                entry.second->OnMessage(nc, wm);
            }
            return;
        }
        case MG_EV_CLOSE: {
            if (MongooseHelper::IsWebsocket(nc)) {
                for (auto entry:kEndPointMap) {
                    entry.second->OnClose(nc);
                }
            }
            return;
        }
        case MG_EV_HTTP_REQUEST: {
            auto *hm = (http_message *) ev_data;
            if (MongooseHelper::IsGET(hm)) {
                if (MongooseHelper::StrEqual(&kHealthUri, &hm->uri)) {
                    MongooseHelper::End_health(nc);
                } else {
                    mg_serve_http(nc, hm, kMgServeHttpOpts);
                }
            } else if (MongooseHelper::IsPOST(hm) && MongooseHelper::StrEqual(&kApiUri, &hm->uri)) {
                auto handler = findHandler(hm);
                if (handler) {
                    RoutingContext rc(nc, hm);
                    handler(rc);
                } else {
                    MongooseHelper::End_404(nc);
                }
            } else {
                MongooseHelper::End_404(nc);
            }
            return;
        }
    }
}

HTTP::~HTTP() {
    kHandlerMap.clear();
    kEndPointMap.clear();
    mg_mgr_free(&kMgMgr);
    kInit = true;
    kSigNum = 0;
}

HTTP &HTTP::AddHandler(const std::string &service, const std::string &action, std::function<void(RoutingContext &)> h) {
    kHandlerMap[service + "." + action] = h;
    return *this;
}

HTTP &HTTP::AddEndPoint(ServerEndPoint &endPoint) {
    kEndPointMap[endPoint.GetUri()] = &endPoint;
    return *this;
}

void HTTP::Start(const HTTP_OPTION &option) {
    if (!kInit) {
        return;
    }

    mg_mgr_init(&kMgMgr, NULL);
    const char *port = option.port.empty() ? "8080" : option.port.c_str();
    if (!option.document_root.empty()) {
        kMgServeHttpOpts.document_root = option.document_root.c_str();
    }
    mg_connection *nc = mg_bind(&kMgMgr, port, mg_ev_handler);
    mg_set_protocol_http_websocket(nc);

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    kInit = false;
    while (kSigNum == 0) {
        mg_mgr_poll(&kMgMgr, 1000);
    }
}
