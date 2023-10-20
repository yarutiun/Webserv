#include "webserv.hpp"

Request::Request(std::string& bufferRef, const Config& config, const Client& client):
	buffer(&bufferRef), // non const buffer from Client
	_config(&config),
	_activeConfig(&config),
	_client(&client),
	_cgiRequest(false),
	_internalScript(no)
{}

Request::Request(const Request& src)
{
	*this = src;
}

Request& Request::operator=(const Request& src)
{
	buffer = src.buffer;
	_config = src._config;
	_activeConfig = src._activeConfig;
	_client = src._client;
	_method = src._method;
	_URL = src._URL;
	_httpProtocol = src._httpProtocol;
	_queryString = src._queryString;
	_headers = src._headers;
	_cookies = src._cookies;
	_requestedHost = src._requestedHost;
	_contentLength = src._contentLength;
	_contentType = src._contentType;
	_sessionID = src._sessionID;
	_directory = src._directory;
	_file = src._file;
	_cgiRequest = src._cgiRequest;
	_cgiExecPath = src._cgiExecPath;
	_cgiIn = src._cgiIn;
	_cgiOut = src._cgiOut;
	_internalScript = src._internalScript;
	_setCookie = src._setCookie;
	_standardFile = src._standardFile;
	_updatedDirectory = src._updatedDirectory;
	_updatedURL = src._updatedURL;
	
	return *this;
}


void Request::process()
{
	parseRequestLine();
	parseRequestHeaders();
	handleSessionID();
	trackSession();
	selectConfig();
	requestError();
	updateVars();
}

std::string Request::prependClassName(std::string function)
{
	return "Request::" + function;
}

void Request::parseRequestLine()
{
	if (buffer->find("\r\n") == std::string::npos)
        perror("Request::parseRequestLine: buffer does not contain \\r\\n");
		// throw ErrorCode(400, MYNAME);
	
	_method = splitEraseStr(*buffer, " ");
	_URL = splitEraseStr(*buffer, " ");
	_httpProtocol = splitEraseStr(*buffer, "\r\n");

	_URL = appendSlash(_URL);
	_directory = _URL.substr(0, _URL.find_last_of("/") + 1);

	size_t	questionMarkPos = _URL.find("?");
	
	if (questionMarkPos != std::string::npos)
	{
		_queryString = _URL.substr(questionMarkPos + 1);
		_URL = _URL.substr(0, questionMarkPos);
	}

	_file = _URL.substr(_URL.find_last_of("/") + 1);
}

void Request::parseRequestHeaders()
{
	if (buffer->find("\r\n\r\n") == std::string::npos)
	{
		if (buffer->size() >= MAX_REQHEADSIZE)
            perror("Request::parseRequestHeaders: buffer size exceeded"); // throw ErrorCode(431, MYNAME);
		else
            perror("Request::parseRequestHeaders: buffer does not contain \\r\\n\\r\\n"); //
			//throw ErrorCode(400, MYNAME);
	}
	_headers = parseStrMap(*buffer, ":", "\r\n", "\r\n");
	
	if (_headers.find("host") != _headers.end())
		_requestedHost = _headers["host"];
	
	if (_headers.find("content-length") != _headers.end())
	{
		std::istringstream iss(_headers["content-length"]);
		iss >> _contentLength;
	}
	else
		_contentLength = 0;
	
	if (_headers.find("content-type") != _headers.end())
		_contentType = _headers["content-type"];

	if (_headers.find("cookie") != _headers.end())
	{
		std::string temp = _headers["cookie"]; // parseStrMap erases from the input string. We want to preserve the cookie header to be able to pass it to CGI
		_cookies = parseStrMap(temp, "=", ";", "");
	}
}

void Request::handleSessionID()
{
	if (_cookies.find(SESSIONID) != _cookies.end() && !_cookies.find(SESSIONID)->second.empty())
	{
		_sessionID = _cookies[SESSIONID];
		_setCookie = false;
	}
	else
	{
		_sessionID = generateSessionId();
		_setCookie = true;
	}
}

void Request::trackSession()
{
	std::string 	logPath = SYS_LOGS + _sessionID + ".log";
	std::ofstream	logFile(logPath.c_str(), std::ios::app);
	
	if (logFile)
	{
		logFile << currentTimeCPP98() << "\t" << _client->getAddr() << "\t" << _method << " " << _URL << "\n";
		logFile.close();
	}
	else
	{
		perror(__FUNCTION__);
		logFile.close();
	}
}

void Request::selectConfig()
{
	if (_requestedHost.empty())
	{
		std::cout << "Empty 'host' header. Running default config." << std::endl;
		_activeConfig = _config;
		_selectedHost = _activeConfig->getNames()[0];
		return;
	}
	
	if (isStringInVec(_requestedHost, _config->getNames()))
	{
		std::cout << "Host '" << _requestedHost << "' belongs to default config." << std::endl;
		_activeConfig = _config;
		_selectedHost = _requestedHost;
		return;
	}

	for (size_t i = 0; i < _config->getAltConfigs().size(); ++i)
	{
		if (isStringInVec(_requestedHost, _config->getAltConfigs()[i].getNames()))
		{
			std::cout << "Host '" << _requestedHost << "' belongs to alternative config #" << i << "." << std::endl;
			_activeConfig = &_config->getAltConfigs()[i];
			_selectedHost = _requestedHost;
			return;
		}
	}

	_activeConfig = _config;
	_selectedHost = _activeConfig->getNames()[0];
}

void Request::requestError()
{
	if (_httpProtocol != HTTPVERSION)
		throw ErrorCode(505, MYNAME);

	if (_method != GET && _method != POST && _method != DELETE)
		throw ErrorCode(501, MYNAME);

	if (_contentLength > _activeConfig->getClientMaxBody())
		throw ErrorCode(413, MYNAME);

	std::map<std::string, locInfo>::const_iterator it = _activeConfig->getLocations()->find(_directory);

	if (it == _activeConfig->getLocations()->end())
	{
		if ((_method == GET || _method == DELETE) && !resourceExists(prependRoot(_URL)))
			throw ErrorCode(404, MYNAME); // can't check before in case of http redir

		throw ErrorCode(403, MYNAME); // should always 404 on a production system to not leak file structure
	}
	
	_locationInfo = it->second;

	if ((_method == GET && !_locationInfo.get)
		|| (_method == POST && !_locationInfo.post)
		|| (_method == DELETE && !_locationInfo.delete_)) 
		throw ErrorCode(405, MYNAME);
}

void Request::updateVars()
{	
	// file extension / CGI
	
	std::string extension = fileExtension(_file);

	if (extension == ".shmang")
	{
		if (_file == "sessionLog.shmang")
			_internalScript = sessionLog;
		/*
		else if (_file == "anotherInternalDynamicContent.shmang")
			_internalScript = anotherDynCont;
		...
		*/
	}
	else if (_activeConfig->getCgiPaths()->find(extension) != _activeConfig->getCgiPaths()->end())
	{
		_cgiExecPath = _activeConfig->getCgiPaths()->find(extension)->second;
		_cgiRequest = true;

		std::stringstream	in, out;
		
		in << SYS_TEMP_CGIIN << _client->getFd();
		_cgiIn = in.str();
		
		out << SYS_TEMP_CGIOUT << _client->getFd() << ".html";
		_cgiOut = out.str();
	}
	
	// standard file

	_standardFile = _locationInfo.std_file;
	if (_standardFile.empty())
		_standardFile = _activeConfig->getStandardFile();
	
	// if (POST) -> upload_redir

	if (_method == POST && !_locationInfo.upload_dir.empty()) // upload_redir supercedes http_redir
	{
		_updatedDirectory = _locationInfo.upload_dir;

		if (!resourceExists(prependRoot(_updatedDirectory)))
			throw ErrorCode(500, MYNAME);
		
		if (_activeConfig->getLocations()->find(_updatedDirectory) == _activeConfig->getLocations()->end())
			throw ErrorCode(403, MYNAME); // upload_redir also has to be set in the config file
		
		_locationInfo = _activeConfig->getLocations()->find(_updatedDirectory)->second; // upload_redir changes locInfo
	}

	// else -> http_redir

	else if (!_locationInfo.http_redir.empty())
		_updatedDirectory = _locationInfo.http_redir; // http_redir does not change locInfo
	else
		_updatedDirectory = _directory;

	// finalize updated vars

	_updatedDirectory = prependRoot(_updatedDirectory);
	_updatedURL = _updatedDirectory + _file;
}

std::string Request::prependRoot(const std::string& path) const
{
	if (path.find('/') == 0)
		return _activeConfig->getRoot() + path.substr(1);
	else
		return path;
}

std::string Request::appendSlash(const std::string& path)
{
	std::string newPath = path;
	
	if (isDirectory(prependRoot(newPath)) && newPath[newPath.size() - 1] != '/')
		newPath.append("/");
	return newPath;
}

void Request::whoIsI() const
{
	std::stringstream ss;
	ss << "---------- Request from Client on fd " << _client->getFd() << " ----------";

	std::string separator(ss.str().size(), '-');

	std::cout	<< "\n" << ss.str() << "\n"
				<< "session id:\t" << _sessionID << "\n"
				<< "requested host:\t" << _requestedHost << "\n"
				<< "active config:\t" << (_activeConfig == _config ? "default config" : "alt config") << "\n"
				<< "selected host:\t" << _selectedHost << "\n"
				<< "method:\t\t" << _method << "\n"
				<< "URL:\t\t" << _URL << "\n"
				<< "dir:\t\t" << _directory << "\n"
				<< "file:\t\t" << _file << "\n"
				<< "content-length:\t" << _contentLength << "\n"
				<< "content-type:\t" << _contentType << "\n"
				<< "standardfile:\t" << _standardFile << "\n"
				<< "dirlisting:\t" << (dirListing() ? "yes" : "no") << "\n"
				<< "updatedURL:\t" << _updatedURL << "\n"
				<< "updatedDir:\t" << _updatedDirectory << "\n"
				<< "query string:\t" << _queryString << "\n"
				<< "cgi request:\t" << (_cgiRequest ? "yes" : "no") << "\n"
				<< separator << std::endl;
}

const Config*	Request::activeConfig() const { return _activeConfig; }

const std::string& Request::method() const { return _method; }

const std::string& Request::URL() const { return _URL; }

const std::string& Request::httpProt() const { return _httpProtocol; }

const std::string& Request::queryString() const { return _queryString; }

const std::map<std::string, std::string>* Request::headers() const { return &_headers; }

const std::map<std::string, std::string>* Request::cookies() const { return &_cookies; }

const std::string& Request::requestedHost() const { return _requestedHost; }

const std::string& Request::selectedHost() const { return _selectedHost; }

size_t Request::contentLength() const { return _contentLength; }

const std::string& Request::contentType() const { return _contentType; }

const std::string& Request::directory() const { return _directory; }

const std::string& Request::file() const { return _file; }

bool Request::dirListing() const
{
	if (_locationInfo.dir_listing == "yes")
		return true;
	else if (_locationInfo.dir_listing == "no")
		return false;
	else if (!_activeConfig->getDefaultDirlisting())
		return false;
	else
		return true;
}

bool Request::cgiRequest() const { return _cgiRequest; }

const std::string& Request::cgiExecPath() const { return _cgiExecPath; }

const std::string& Request::cgiIn() const { return _cgiIn; }

const std::string& Request::cgiOut() const { return _cgiOut; }

const dynCont& Request::internalScript() const { return _internalScript; }

bool Request::setCookie() const { return _setCookie; }

const std::string& Request::sessionID() const { return _sessionID; }

const std::string& Request::standardFile() const { return _standardFile; }

const std::string& Request::updatedDir() const { return _updatedDirectory; }

const std::string& Request::updatedURL() const { return _updatedURL; }

const locInfo* Request::locationInfo() const { return &_locationInfo; }

std::string Request::statusPagePath(int code) const
{
	std::map<int, std::string>::const_iterator codePath = _activeConfig->getStatusPagePaths()->find(code);
	
	if (codePath == _activeConfig->getStatusPagePaths()->end())
		return "";
	return prependRoot(codePath->second);
}

const std::string& Request::root() const { return _activeConfig->getRoot(); }
