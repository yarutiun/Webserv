#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webserv.hpp"
class Config;

class Server{
    public:
        Server();
        bool poll();

    private:
//        std::vector<Config>    _config_;
        std::vector<struct pollfd>     _pollStructs_;
        std::vector<struct pollfd>::iterator _pollStruct_;
};
#endif