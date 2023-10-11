#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../webserv.hpp"
class Client
{
    public:
        int getFd();
    
    private:
        int _fd_;
};

#endif
