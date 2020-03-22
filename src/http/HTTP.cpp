//
// Created by ixtf on 2020/3/22.
//

#include "HTTP.hpp"

static bool kInit = true;
static mg_str kMethodGET = MG_MK_STR("GET");
static mg_str kMethodPOST = MG_MK_STR("POST");
static mg_str kHealthUri = MG_MK_STR("/health");
static mg_str kApiUri = MG_MK_STR("/api");
static mg_str kService = MG_MK_STR("X-SERVICE");
static mg_str kAction = MG_MK_STR("X-ACTION");
static std::map<std::string, std::function<void(RoutingContext &)>> kHandlerMap;

static int kSigNum = 0;
static mg_mgr kMgMgr;
static mg_serve_http_opts kMgServeHttpOpts;

inline static void signal_handler(int sig_num) {
    signal(sig_num, signal_handler);
    kSigNum = sig_num;
}

static void serve_websocket(mg_connection *nc, const websocket_message *wm) {
    mg_connection *c;
    char buf[500];
    char addr[32];
    mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);

    for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
        if (c == nc) {
            continue;
        }
        mg_send_websocket_frame(c, WEBSOCKET_OP_TEXT, buf, strlen(buf));
    }
}

static std::function<void(RoutingContext &)> find_handler(http_message *hm) {
    std::string service;
    std::string action;
    for (int i = 0; i < MG_MAX_HTTP_HEADERS && hm->header_names[i].len > 0; i++) {
        struct mg_str hn = hm->header_names[i];
        struct mg_str hv = hm->header_values[i];
//        printf("%.*s : %.*s \n", (int) hn.len, hn.p, (int) hv.len, hv.p);
        if (mg_str_equal(&kService, &hn)) {
            service = std::string(hv.p, hv.len);
            continue;
        }
        if (mg_str_equal(&kAction, &hn)) {
            action = std::string(hv.p, hv.len);
            continue;
        }
    }
    if (service.length() == 0 || action.length() == 0) {
        return NULL;
    }
    std::string key = service + "." + action;
    auto iter = kHandlerMap.find(key);
    if (iter == kHandlerMap.end()) {
        return NULL;
    };
    return iter->second;
}

static void mg_ev_handler(mg_connection *nc, int ev, void *ev_data) {
    switch (ev) {
        case MG_EV_WEBSOCKET_FRAME: {
            websocket_message *wm = (websocket_message *) ev_data;
            serve_websocket(nc, wm);
            return;
        }
        case MG_EV_HTTP_REQUEST: {
            http_message *hm = (http_message *) ev_data;
            if (mg_str_equal(&kMethodGET, &hm->method)) {
                if (mg_str_equal(&kHealthUri, &hm->uri)) {
                    RoutingResponse::End_health(nc);
                } else {
                    mg_serve_http(nc, hm, kMgServeHttpOpts);
                }
            } else if (mg_str_equal(&kMethodPOST, &hm->method) && mg_str_equal(&kApiUri, &hm->uri)) {
                auto handler = find_handler(hm);
                if (handler == NULL) {
                    RoutingResponse::End_404(nc);
                } else {
                    RoutingContext rc(nc, hm);
                    handler(rc);
                }
            } else {
                RoutingResponse::End_404(nc);
            }
            return;
        }
    }
}

HTTP::~HTTP() {
    mg_mgr_free(&kMgMgr);
}

void HTTP::AddHandler(const std::string &service, const std::string &action, std::function<void(RoutingContext &)> h) {
    auto key = service + "." + action;
    kHandlerMap[key] = h;
}

void HTTP::Start(const HTTP_OPTION &option) {
    if (!kInit) {
        return;
    }
    mg_mgr_init(&kMgMgr, NULL);
    mg_connection *nc = mg_bind(&kMgMgr, option.port.c_str(), mg_ev_handler);
    mg_set_protocol_http_websocket(nc);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    kInit = false;
    while (kSigNum == 0) {
        mg_mgr_poll(&kMgMgr, 1000);
    }
}
