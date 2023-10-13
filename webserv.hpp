#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <sys/socket.h>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include<vector>
#include<csignal>


#include "Server/Server.hpp"
class Server;

#include "Config/configFile.hpp"
class Config;

# include <sys/poll.h>

#include "defines.hpp"

#include "Utils/utils.hpp"
//global variable//
extern volatile sig_atomic_t				signum;

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include<sys/types.h>


#include"Binding/Binding.hpp"
class Binding;

#include"netinet/in.h"
#include"arpa/inet.h"
#include<sstream>
#include"unistd.h"

#include "Client/Client.hpp"
class Client;

#endif