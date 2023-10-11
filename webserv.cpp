#include "webserv.hpp"

void sigHandler(int sig)
{
    std::cout << "Signal received: " << sig << std::endl;
    signum = 1;
    
}
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

    }
    return(0);
}