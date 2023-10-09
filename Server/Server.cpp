#include"Server.hpp"

Server::Server() {
    std::string path = "./sampleConfig.conf";

}

bool Server::poll() {
    // monitor multiple descriptors for events. ::poll system call allows to wait for events on multiple fds
    // wo blocking the program (async)
    if (::poll(_pollStructs_.data(), _pollStructs_.size(), -1) == -1)
    {
        if (!signum)
            perror("poll error");
    }
    if (signum)
        return false;
    return true;
}