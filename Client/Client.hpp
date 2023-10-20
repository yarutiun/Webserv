#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../webserv.hpp"
class Request;
class Response;

class Client
{
    public:
        Client(std::vector<struct pollfd>::iterator pollStruct, int fd, sockaddr_in address);
        int getFd() const;
        void incomingData(std::vector<struct pollfd>::iterator pollStruct);
        void receive();
        bool outgoingData();
        void newRequest();
        const char *getAddr() const;
    
    private:
        std::string _buffer_;
        std::vector<struct pollfd>::iterator _pollStruct_;
        int _fd_;
        sockaddr_in _address_;
        Request *_request_;
        // Response *_response_;
};

#endif
