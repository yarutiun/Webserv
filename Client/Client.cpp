#include "Client.hpp"


Client::Client(std::vector<struct pollfd>::iterator pollstruct, int fd, sockaddr_in address): _pollStruct_(pollstruct), _fd_(fd), _address_(address)
{
    std::cout << "Client created" << _fd_ << _address_.sin_addr.s_addr << std::endl;
}

int Client::getFd() const
{
    return(_fd_);
}

void Client::incomingData(std::vector<struct pollfd>::iterator pollstruct)
{
    _pollStruct_ = pollstruct;
    receive();
    // std::cout << "Client " << _fd_ << " says: " << _buffer_ << std::endl;
    if (!_request_)
        newRequest();
}

void Client::receive()
{
    char buff[RECV_CHUNK_SIZE];

    int bytesReceived = recv(_fd_, buff, RECV_CHUNK_SIZE, 0);
    if (bytesReceived <= 0)
        perror("recv error"); // 
    _buffer_.append(buff, bytesReceived);
}

bool Client::outgoingData()
{
    ////////
    return true; // should be amended 
}

void Client::newRequest()
{
    _request_ = new Request(_buffer_, *this); // change later
    _request_->process();
}

const char *Client::getAddr() const
{
    return(inet_ntoa(_address_.sin_addr));
}