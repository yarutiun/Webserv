#ifndef BINDING_HPP
#define BINDING_HPP

#include"../webserv.hpp"

class Binding
{
    public:
        Binding(const Configuration &conf);
        sockaddr_in *addrOfSocketAddr();
        void    setFd(int fd);
        void    whoIs(); //nam e

    private:
        sockaddr_in _addr_;
        int         _fd_;
        const Configuration *_conf_;
};



#endif