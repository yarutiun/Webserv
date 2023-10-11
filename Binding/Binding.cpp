#include"Binding.hpp"

Binding::Binding()
{
    _addr_.sin_family = AF_INET;
    _addr_.sin_addr.s_addr = INADDR_ANY;
    _addr_.sin_port = htons(8080);
}

sockaddr_in *Binding::addrOfSocketAddr()
{
    return(&_addr_);
}

void Binding::setFd(int fd)
{
    _fd_ = fd;
}

void Binding::whoIs()
{
    std::stringstream title;
    std::string dashes(15, '-');
    title << dashes << "Binding " << inet_ntoa(_addr_.sin_addr) << ":" << ntohs(_addr_.sin_port) << dashes;
    std::string sep(title.str().size(), '-');
    std::cout << "\n" << title.str() << "\n";
    //whoisConf...
    std::cout << sep << "\n";
}