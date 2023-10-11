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
        perror("socket error"); //errors to be handled smarter
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
        perror("setsockopt error");
    if (fcntl(listeningSocket, F_SETFL, O_NONBLOCK) == -1)
        perror("fcntl error");
    if (bind(listeningSocket, (struct sockaddr*)newBinding->addrOfSocketAddr(), sizeof(sockaddr_in)) == -1)
        perror("bind error");
    if (listen(listeningSocket, SOMAXCONN) == -1)
        perror("listen error");
    
    addPollStruct(listeningSocket, POLLIN);
    newBinding->setFd(listeningSocket);
    _binds_.push_back(newBinding);
    newBinding->whoIs();
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

void Server::acceptNewClients()
{
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int newClientFd = accept(_pollStructs_[0].fd, (struct sockaddr *)&clientAddr, &clientAddrSize); //
    if (newClientFd == -1)
        perror("accept error");
    else
    {
        if (fcntl(newClientFd, F_SETFL, O_NONBLOCK) == -1)
            perror("client fcntl error");
        addPollStruct(newClientFd, POLLIN | POLLHUP);
        // _clients_.push_back()
    }
}


void Server::handleClients()
{

}

void Server::addPollStruct(int fd, short events)
{
    struct pollfd newPollStruct;
    newPollStruct.fd = fd;
    newPollStruct.events = events;
    newPollStruct.revents = 0;
    _pollStructs_.push_back(newPollStruct);
    // _pollStruct_ = _pollStructs_.end() - 1;
}

std::vector<Client *>::iterator Server::getClientByFd(int fd)
{
    std::vector<Client *>::iterator it = _clients_.begin();
    while (it != _clients_.end() && (*it)->getFd() != fd)
        it++;
    if (it == _clients_.end())
    {
        std::cerr << "Client not found" << std::endl;
        throw std::runtime_error("Client not found"); //
    }
    return it;
}