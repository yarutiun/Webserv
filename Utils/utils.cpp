#include"utils.hpp"


void sigHandler(int sig)
{
    std::cout << "Signal received: " << sig << std::endl;
    signum = 1;
    
}