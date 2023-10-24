#include "../webserv.hpp"

Server::Server(int argc, char **argv) {
    std::string path = "configs/default.conf";
    if (argc > 1)
        path = argv[1];
    ConfigFileParser configFileParser(path.c_str());
    _configs_ = configFileParser.getConfigs();
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
    _pollStruct_ = _pollStructs_.begin();
    while (_pollStruct_ != _pollStructs_.end())
    {
        while (true)
        {
            sockaddr_in clientAddr;
            socklen_t clientAddrSize = sizeof(clientAddr);
            int newClientFd = accept(_pollStructs_[0].fd, (struct sockaddr *)&clientAddr, &clientAddrSize); //
            if (newClientFd == -1)
            {
                std::cout << errno;
                if (errno == EWOULDBLOCK)
                    return ;
                acceptError(newClientFd);
            }
            if (fcntl(newClientFd, F_SETFL, O_NONBLOCK) == -1)
                acceptError(newClientFd);
                    // perror("client fcntl error");
            addPollStruct(newClientFd, POLLIN | POLLHUP);
            _clients_.push_back(new Client(_configs_[0], _pollStruct_, newClientFd, clientAddr));
        }
        _pollStruct_++; //
    }
}


void Server::handleClients()
{
    _pollStruct_ = _pollStructs_.begin() + _binds_.size();
    while(_pollStruct_ != _pollStructs_.end())
    {
        std::cout << "handling client" << std::endl;
        _client_ = getClientByFd(_pollStruct_->fd);
        if (pollhup())
            continue;
        if (pollin())
            continue;
        if (pollout())
            continue;
        
        _pollStruct_++;
    }
    //try - catch


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

//Checks if a "hang-up" event occurred on the client socket (POLLHUP). 
//If it did, it handles the hang-up event by closing the client connection.
bool Server::pollhup()
{
    if (_pollStruct_->revents & POLLHUP)
    {
        std::cout << "Client " << _pollStruct_->fd << " disconnected" << std::endl;
        closeClientConnection();
        return true;
    }
    return false;

}

void Server::closeClientConnection() // add message to this function
{
    std::cout << "Closing Client on fd " << _pollStruct_->fd << std::endl;
    close(_pollStruct_->fd);
    _pollStruct_ = _pollStructs_.erase(_pollStruct_); // erase returns the next valid position
    delete *_client_;
    _clients_.erase(_client_);
}

bool Server::pollin()
{
    if (_pollStruct_->revents & POLLIN)
    {
        (*_client_)->incomingData(_pollStruct_);
        ++_pollStruct_;
        return true;
    }
    return false;
}

bool    Server::pollout()
{
    if (_pollStruct_->revents & POLLOUT)
    {
        if ((*_client_)->outgoingData())
            return (closeClientConnection(), true);
        ++_pollStruct_;
        return true;
    }
    return false;
}

void Server::acceptError(int fd)
{
    close(fd);
    throw std::runtime_error("accept error");
}


