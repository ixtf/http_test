ADD_EXECUTABLE(http_test main.cpp
        http/HTTP.cpp http/HTTP.hpp
        http/RoutingContext.cpp http/RoutingContext.hpp
        http/ServerEndPoint.cpp http/ServerEndPoint.hpp
        http/MongooseHelper.cpp http/MongooseHelper.hpp
        http/mongoose.c http/mongoose.h
        ChatServerEndPoint.cpp ChatServerEndPoint.hpp ImageServerEndPoint.cpp ImageServerEndPoint.hpp)