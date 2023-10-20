#include "webserv.hpp"

volatile sig_atomic_t signum = 0;

int main(void)
{
    std::signal(SIGINT, sigHandler);
    std::signal(SIGTERM, sigHandler);

    Server webserv;

    webserv.bindListeningSocket();
    while(webserv.poll())
    {
        webserv.acceptNewClients();
        webserv.handleClients();

    }
    return(0);
}