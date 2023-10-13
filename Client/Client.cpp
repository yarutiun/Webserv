#include "Client.hpp"

int Client::getFd()
{
    return(_fd_);
}

void Client::incomingData(std::vector<struct pollfd>::iterator pollstruct)
{
    _pollStruct_ = pollstruct;
    receive();
    std::cout << "Client " << _fd_ << " says: " << _buffer_ << std::endl;
}

void Client::receive()
{
    char buff[RECV_CHUNK_SIZE];

    int bytesReceived = recv(_fd_, buff, RECV_CHUNK_SIZE, 0);
    if (bytesReceived <= 0)
        perror("recv error"); // 
    _buffer_.append(buff, bytesReceived);
}