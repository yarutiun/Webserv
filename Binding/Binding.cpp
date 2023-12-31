#include"Binding.hpp"

Binding::Binding(const Configuration &conf): _conf_(&conf)
{
	_fd_ = -1;
	_addr_.sin_family = AF_INET;
	_addr_.sin_addr.s_addr = conf.getHost();
	_addr_.sin_port = conf.getPort();
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
    _conf_->whoIsI();
    std::cout << sep << "\n";
}