#include "webserv.hpp"

volatile sig_atomic_t signum = 0;
std::map<std::string, std::string>	mimeTypes = initMimeTypesMap();

int main(int argc, char **argv)
{
    std::signal(SIGINT, sigHandler);
    std::signal(SIGTERM, sigHandler);

    Server webserv(argc, argv);

    webserv.launchBinds();
    while(webserv.poll())
    {
        webserv.acceptNewClients();
        webserv.handleClients();

    }
    return(0);
}