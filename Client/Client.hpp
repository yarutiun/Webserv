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
        int _fd_;
        std::vector<struct pollfd>::iterator _pollStruct_;
        std::string _buffer_;
        sockaddr_in _address_;
};

#endif
