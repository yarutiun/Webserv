#include "webserv.hpp"

volatile sig_atomic_t signum = 0;

int main(int argc, char **argv)
{
    std::signal(SIGINT, sigHandler);
    std::signal(SIGTERM, sigHandler);

    Server webserv(argc, argv);

    webserv.bindListeningSocket();
    while(webserv.poll())
    {
        webserv.acceptNewClients();
        webserv.handleClients();

    }
    return(0);
}