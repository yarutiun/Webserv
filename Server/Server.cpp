#include "../webserv.hpp"

Server::Server(int argc, char **argv) {
    std::string path = "system/configs/default.conf";
    if (argc > 1)
        path = argv[1];
    ConfigFileParser configFileParser(path.c_str());
    _configs_ = configFileParser.getConfigs();
}

/*function is responsible for iterating over server configurations, attempting to bind sockets for each configuration
*/
void Server::launchBinds()
{
    for(size_t i = 0; i < _configs_.size(); ++i)
    {
        try
        {
            bindListeningSocket((_configs_)[i]);
        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    if (_binds_.empty())
    {
        std::cerr << E_S_NOBINDINGS << std::endl;
        shutdown();
    }
    std::cout << I_S_BINDINGS << std::endl;
}

void Server::bindListeningSocket(const Configuration &config)
{
    Binding *newBinding = new Binding(config);
    int listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt = 1;

    if (listeningSocket == -1)
		bindError(listeningSocket, newBinding);
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) == -1)
		bindError(listeningSocket, newBinding);
    if (fcntl(listeningSocket, F_SETFL, O_NONBLOCK) == -1)
		bindError(listeningSocket, newBinding);
    if (bind(listeningSocket, (struct sockaddr*)newBinding->addrOfSocketAddr(), sizeof(sockaddr_in)) == -1)
		bindError(listeningSocket, newBinding);
    if (listen(listeningSocket, SOMAXCONN) == -1)
		bindError(listeningSocket, newBinding);
    
    addPollStruct(listeningSocket, POLLIN);
    newBinding->setFd(listeningSocket);
    _binds_.push_back(newBinding); //The _binds_ attribute represents the successful bindings or connections established by the server. 
    //Each successful binding corresponds to a potential client connection.
    newBinding->whoIs();
}

/* The poll function is used to wait for an event on any of the file descriptors in the _pollStructs array. 
It blocks until either an event occurs or a signal is received. 
The poll function updates the revents field in the pollfd structures to indicate which events have occurred.
*/

bool Server::poll() 
{
    // monitor multiple descriptors for events. ::poll system call allows to wait for events on multiple fds
    // wo blocking the program
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
    for (size_t i = 0; i < _binds_.size(); ++i)
    {
        if (!(_pollStructs_[i].revents & POLLIN))
            continue;

        while (true)
        {
            sockaddr_in clientAddr;
            socklen_t clientAddrSize = sizeof(clientAddr);
            int newClientFd = accept(_pollStructs_[i].fd, (struct sockaddr *)&clientAddr, &clientAddrSize);
            if (newClientFd == -1)
            {
                if (errno == EWOULDBLOCK)
                    return ;
                acceptError(newClientFd);
            }
            if (fcntl(newClientFd, F_SETFL, O_NONBLOCK) == -1)
                acceptError(newClientFd);
            
            addPollStruct(newClientFd, POLLIN | POLLHUP);
            _clients_.push_back(new Client(_configs_[i], _pollStructs_, newClientFd, clientAddr));
        }
    }
}


void Server::handleClients()
{
    _pollStruct_ = _pollStructs_.begin() + _binds_.size();
    while(_pollStruct_ != _pollStructs_.end())
    {
        try
        {
            _client_ = getClientByFd(_pollStruct_->fd);
            if (pollhup())
                continue;
            if (pollin())
                continue;
            if (pollout())
                continue;
        } catch(const ErrCode &e)
        {
            std::cerr << e.what() << std::endl;
            (*_client_)->sendStatusPage(e.getCode());
        }
        catch(const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            closeClientConnection(CLOSE_EXCPT);
            continue;
        }
        ++_pollStruct_;
    }
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
        ++it;
    if (it == _clients_.end())
    {
        std::cerr << "Client not found" << std::endl;
        throw std::runtime_error(__FUNCTION__);
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
        closeClientConnection(CLOSE_POLLHUP);
        return true;
    }
    return false;

}

void Server::closeClientConnection(std::string msg)
{
    std::cout << "Closing Client on fd " << _pollStruct_->fd << ": " << msg << std::endl;
    close(_pollStruct_->fd);
    _pollStruct_ = _pollStructs_.erase(_pollStruct_); // erase returns the next valid position
    delete *_client_;
    _clients_.erase(_client_);
}

//It checks if a client's file descriptor is ready for input (reading), and if it is, it reads incoming data from the 
//client and increments the _pollStruct pointer to prepare for the next poll.
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

//t checks if a client's file descriptor is ready for output (writing), and if there is no outgoing data to send, 
//it closes the client connection and returns true. Otherwise, it returns false.
bool    Server::pollout()
{
    if (_pollStruct_->revents & POLLOUT)
    {
        if (!(*_client_)->outgoingData())
            return (closeClientConnection(CLOSE_DONE), true);
        //++_pollStruct_; why dont we go to next iteration here?
        return true;
    }
    return false;
}

void Server::acceptError(int fd)
{
    close(fd);
    throw std::runtime_error("accept error");
}

void Server::shutdown()
{
	std::cout << "\nShutdown." << std::endl;

	std::cout << "\nClosing " << _pollStructs_.size() << (_pollStructs_.size() == 1 ? " socket:" : " sockets:") << std::endl;
	for (std::vector<pollfd>::iterator it = _pollStructs_.begin(); it != _pollStructs_.end(); ++it)
	{
		std::cout << "\tClosing socket fd " << it->fd << "." << std::endl;
		close(it->fd);
	}

	std::cout << "\nDeleting " << _binds_.size() << (_binds_.size() == 1 ? " Binding:" : " Bindings:") << std::endl;
	for (size_t i = 0; !_binds_.empty(); ++i)
	{
		std::cout << "\tDeleting Binding " << i << "." << std::endl;
		delete _binds_[0];
		_binds_.erase(_binds_.begin());
	}

	if (_clients_.empty())
		std::cout << "\nNo Clients connected, nothing to delete." << std::endl;
	else
	{
		std::cout << "\nDeleting " << _clients_.size() << (_clients_.size() == 1 ? " Client:" : " Clients:") << std::endl;
		for (size_t i = 0; !_clients_.empty(); ++i)
		{
			std::cout << "\tDeleting Client " << i << "." << std::endl;
			delete _clients_[0];
			_clients_.erase(_clients_.begin());
		}
	}
}

void Server::bindError(int fd, Binding *newBinding)
{
    if (fd == -1)
        close(fd);
    delete newBinding;
    throw std::runtime_error(strerror(errno));
}