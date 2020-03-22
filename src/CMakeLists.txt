ADD_EXECUTABLE(http_test main.cpp
        http/HTTP.cpp http/HTTP.hpp
        http/RoutingContext.cpp http/RoutingContext.hpp
        http/mongoose.c http/mongoose.h
        )