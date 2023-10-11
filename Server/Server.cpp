#include"Server.hpp"

Server::Server() {
    std::string path = "./sampleConfig.conf";

}

void Server::launchBinds()
{
    bindListeningSocket();
}

void Server::bindListeningSocket()
{
    Binding *newBinding = new Binding();
    int listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;

    if (listeningSocket == -1)
        perror("socket error");
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
        perror("setsockopt error");
    if (fcntl(listeningSocket, F_SETFL, O_NONBLOCK) == -1)
        perror("fcntl error");
    if (bind(listeningSocket, (struct sockaddr*)newBinding->addrOfSocketAddr(), sizeof(sockaddr_in)) == -1)
        perror("bind error");
    if (listen(listeningSocket, SOMAXCONN) == -1)
        perror("listen error");

}

bool Server::poll() 
{
    // monitor multiple descriptors for events. ::poll system call allows to wait for events on multiple fds
    // wo blocking the program (async)
    if (::poll(_pollStructs_.data(), _pollStructs_.size(), -1) == -1)
    {
        if (!signum)
            perror("poll error");
    }
    if (signum)
        return false;
    return true;
}

