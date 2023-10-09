#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <sys/socket.h>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include "Client/Client.hpp"
#include<vector>
#include<csignal>
//class Client;

#include "Server/Server.hpp"
class Server;

#include "Config/configFile.hpp"
class Config;

# include <sys/poll.h>

//#include "defines.hpp"

#include "Utils/utils.hpp"
//global variable//
extern volatile sig_atomic_t				signum;


#endif