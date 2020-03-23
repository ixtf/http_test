//
// Created by ixtf on 2020/3/22.
//

#ifndef HTTP_TEST_HTTP_HPP
#define HTTP_TEST_HTTP_HPP

#include "RoutingContext.hpp"
#include "ServerEndPoint.hpp"

typedef struct {
    std::string port;
    std::string document_root;
    std::string health_uri;
    std::string service_param;
    std::string action_param;
} HTTP_OPTION;

class HTTP {
public:
    ~HTTP();

    HTTP(HTTP const &) = delete;

    void operator=(HTTP const &) = delete;

    static HTTP &getInstance() {
        static HTTP instance;
        return instance;
    }

    HTTP &AddHandler(const std::string &service, const std::string &action, std::function<void(RoutingContext &rc)> h);

    HTTP &AddEndPoint(ServerEndPoint &endPoint);

    void Start(void) {
        HTTP_OPTION option;
        Start(option);
    }

    void Start(const HTTP_OPTION &option);

private:
    HTTP() {}
};


#endif //HTTP_TEST_HTTP_HPP
