#include "Client.hpp"

int Client::getFd()
{
    return(_fd_);
}

void Client::incomingData(std::vector<struct pollfd>::iterator pollstruct)
{
    
}