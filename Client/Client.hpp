#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../webserv.hpp"

class Client
{
    public:
        Client(std::vector<struct pollfd>::iterator pollStruct, int fd, sockaddr_in address);
        int getFd();
        void incomingData(std::vector<struct pollfd>::iterator pollStruct);
        void receive();
    
    private:
        std::string _buffer_;
        std::vector<struct pollfd>::iterator _pollStruct_;
        int _fd_;
        sockaddr_in _address_;
};

#endif
