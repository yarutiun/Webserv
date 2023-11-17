#ifndef SERVER_HPP
#define SERVER_HPP

#include "../webserv.hpp"
class Binding;
class Client;
class Configuration;

class Server{
    public:
        Server(int argc, char **argv);
        bool poll();
        void launchBinds();
        void bindListeningSocket(const Configuration &conf);
        void acceptNewClients();
        void addPollStruct(int fd, short events);
        void handleClients();
        std::vector<Client *>::iterator getClientByFd(int fd);
        bool pollhup();
        bool pollin();
        bool pollout();
        void closeClientConnection(std::string msg);
        void acceptError(int newClientFd);
        void shutdown();
        void bindError(int Binds, Binding *newBinding);

    private:
        std::vector<Configuration>    _configs_;
        std::vector<Binding *>   _binds_;
        std::vector<Client *>   _clients_;
        std::vector<struct pollfd>     _pollStructs_;
        std::vector<struct pollfd>::iterator _pollStruct_;
        std::vector<Client *>::iterator _client_;
};
#endif