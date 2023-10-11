#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webserv.hpp"

class Server{
    public:
        Server();
        bool poll();
        void launchBinds();
        void bindListeningSocket();


    private:
//        std::vector<Config>    _config_;
        std::vector<Binding *>   _binds_;
        std::vector<struct pollfd>     _pollStructs_;
        std::vector<struct pollfd>::iterator _pollStruct_;
};
#endif