#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../webserv.hpp"
class Client
{
    public:
        int getFd();
        void incomingData(std::vector<struct pollfd>::iterator pollStruct);
    
    private:
        int _fd_;
        std::vector<struct pollfd>::iterator _pollStruct_;
};

#endif
