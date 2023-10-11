#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webserv.hpp"
class Binding;
class Client;

class Server{
    public:
        Server();
        bool poll();
        void launchBinds();
        void bindListeningSocket();
        void acceptNewClients();
        void addPollStruct(int fd, short events);
        void handleClients();
        std::vector<Client *>::iterator getClientByFd(int fd);
        bool pollhup();
        void closeClientConnection();

    private:
//        std::vector<Config>    _config_;
        std::vector<Binding *>   _binds_;
        std::vector<Client *>   _clients_;
        std::vector<struct pollfd>     _pollStructs_;
        std::vector<struct pollfd>::iterator _pollStruct_;
        std::vector<Client *>::iterator _client_;
};
#endif