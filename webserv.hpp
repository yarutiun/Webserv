#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <sys/socket.h>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <csignal>
#include <sstream>


typedef struct locationInformation
{
	bool			get;
	bool			post;
	bool			delete_;
	std::string		dir_listing;
	std::string		http_redir;
	std::string		upload_dir;
	std::string		std_file;
}	locInfo;

typedef enum dynamicContentSelector
{
	no,
	statusPage,
	dirListing,
	sessionLog
}	dynCont;

// #include"Config/configFile.hpp"
// class Config;


#include "Configuration/Configuration.hpp"
class Config;

#include "Configuration/ConfigFileParser.hpp"
class ConfigFileParser;

#include "Server/Server.hpp"
class Server;

#include "Request/Request.hpp"
class Request;

// #include "Response/Response.hpp"
// class Response;

#include "defines.hpp"
#include <sys/poll.h>


#include "Utils/utils.hpp"
//global variable//
extern volatile sig_atomic_t				signum;
extern std::map<std::string, std::string>	mimeTypes;

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

// #include "Response/File.Response.hpp"
// class File;

// #include"Response/DynContent.Response.hpp"
// class DynContent;











#include <fstream>



#include <sys/stat.h>
#include "Exceptions/ErrCode.hpp"
class ErrCode;

#include<dirent.h>   // For opendir and other directory-related functions
#include<unistd.h> // fork, execve, close, exit..
#include<ctime>
#include<cstdlib> // for EXIT_FAILURE
#endif