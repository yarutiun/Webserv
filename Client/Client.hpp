#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../webserv.hpp"
class Request;
class Response;

class Client
{
    public:
        Client(const Configuration &config, std::vector<struct pollfd> &pollstructs, int fd, sockaddr_in address);
        Client &operator=(const Client& src);
        ~Client();
        Client(const Client& src);
        int getFd() const;
        void incomingData(std::vector<struct pollfd>::iterator pollStruct);
        void receive();
        bool outgoingData();
        void newRequest();
        void newResponse(std::string sendPath);
        void newResponse(int statusCode);
        void newResponse(dynCont &dynContent);
        const char *getAddr() const;
        void        handleGet();
        void        handlePost();
        void        handleDelete();
        bool        handleCGI();
        void        launchChild();
        void        makeEnv();
        void        sendStatusPage(int code);
        std::string prependClassName(std::string function);
        void        whoIsI() const;



    
    private:
        std::string                             _buffer_;
        const Configuration                     &_config_;
        std::vector<struct pollfd>              _pollStructs_;
        std::vector<struct pollfd>::iterator    _pollStruct_;
        int                                     _fd_;
        sockaddr_in                             _address_;
        Request                                 *_request_;
        Response                                *_response_;
        bool                                    _append_;
        size_t                                  _bytesWritten_;
        bool                                    _cgiInProgress_;
        std::vector<std::string>                _argVVecStr_;
        std::vector<char*>                      _argVVec_;
        std::vector<std::string>                _envVecStr_;
        std::vector<char*>                      _envVec_;
        pid_t                                   _pid_;
        time_t                                  _time_;
};

#endif
