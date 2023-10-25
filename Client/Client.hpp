#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../webserv.hpp"
class Request;
class Response;

class Client
{
    public:
        Client(const Configuration &conf, std::vector<struct pollfd>::iterator pollStruct, int fd, sockaddr_in address);
        int getFd() const;
        void incomingData(std::vector<struct pollfd>::iterator pollStruct);
        void receive();
        bool outgoingData();
        void newRequest();
        void newResponse(const std::string &sendPath); // made it const ?!?
        void newResponse(int statusCode);
        void newResponse(dynCont &dynContent);
        const char *getAddr() const;
        void        handleGet();
        void        handlePost();
        void        handleDelete();
        bool        handleCGI();
        void        launchChild();
        void        makeEnv();

    
    private:
        std::string _buffer_;
        const Configuration     &_config_;
        std::vector<struct pollfd>::iterator _pollStruct_;
        int         _fd_;
        sockaddr_in _address_;
        Request     *_request_;
        Response    *_response_;
        bool       _append_;
        size_t      _bytesWritten_;
        bool        _cgiInProgress_;
        std::vector<std::string>    _argVVecStr_;
        std::vector<char*>          _argVVec_;
};

#endif
