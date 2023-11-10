#include "Client.hpp"

Client::Client(const Configuration &config, std::vector<struct pollfd>::iterator pollstruct, int fd, sockaddr_in address): _config_(config), 
_pollStruct_(pollstruct), /// serious damage!!!????!!!
_fd_(fd), 
_address_(address),
_request_(NULL),
_response_(NULL),
_append_(false),
_bytesWritten_(0),
_cgiInProgress_(false),
_time_(0)
{
}

Client::Client(const Client& src):
	_config_(src._config_),
	_pollStructs_(src._pollStructs_)
{
	*this = src;
}

Client::~Client()
{
	if (_request_)
		delete _request_;
	
	if (_response_)
		delete _response_;
}

Client& Client::operator=(const Client& src)
{
	if (src._request_ != NULL)
		_request_ = new Request(*src._request_);
	else
		_request_ = NULL;
	
	if (src._response_ != NULL)
		_response_ = src._response_->clone();
	else
		_response_ = NULL;
	
	_fd_ = src._fd_;
	_address_ = src._address_;
	_buffer_ = src._buffer_;
	_pollStruct_ = src._pollStruct_;

	_append_ = src._append_;
	_bytesWritten_ = src._bytesWritten_;
	
	_pid_ = src._pid_;
	_time_ = src._time_;
	_cgiInProgress_ = src._cgiInProgress_;
	
	_envVec_ = src._envVec_;
	_envVecStr_ = src._envVecStr_;
	_argVVec_ = src._argVVec_;
	_argVVecStr_ = src._argVVecStr_;

	return *this;
}

int Client::getFd() const
{
    return(_fd_);
}

void Client::incomingData(std::vector<struct pollfd>::iterator pollstruct)
{
    _pollStruct_ = pollstruct;
    receive();

    if (!_request_)
        newRequest();
    if (_request_->internalScript())
		newResponse(_request_->internalScript());
    else if ( _request_->method() == GET)
        handleGet();
    else if (_request_->method() == POST)
        handlePost();
    else if(_request_->method() == DELETE)
        handleDelete();
}

void Client::receive()
{
    char buff[RECV_CHUNK_SIZE];

    int bytesReceived = recv(_fd_, buff, RECV_CHUNK_SIZE, 0);
    if (bytesReceived <= 0)
        throw CloseConnection("MYNAME", E_RECV); //
    _buffer_.append(buff, bytesReceived);
}

bool Client::outgoingData()
{
    if (_cgiInProgress_)
    {
        if (handleCGI())
            return true; // cgi not done
    }    
    return _response_->send(_fd_);
}

void Client::newRequest()
{
    _request_ = new Request(_buffer_, _config_, *this); // ch
    _request_->process();
    _request_->whoIsI();
}

const char *Client::getAddr() const
{
    return(inet_ntoa(_address_.sin_addr));
}

void Client::handleGet()
{
	if (_request_->cgiRequest())
	{
		handleCGI();
		return;
	}
    if(isDirectory(_request_->updatedURL()))
    {
        std::string stdFile = _request_->updatedURL() + _request_->standardFile();
        if (resourceExists(stdFile))
            newResponse(stdFile);
        else if (_request_->dirListing())
            newResponse(dirListing);
        else
            newResponse(404);
    }
    else
    {
        if (resourceExists(_request_->updatedURL()))
            newResponse(_request_->updatedURL());
        else
            newResponse(404);
    }
}


void Client::newResponse(std::string path)
{
    if (_response_)
        delete _response_;

    _response_ = new File(200, path, *_request_);
    _pollStruct_->events = POLLOUT | POLLHUP;
}

void Client::newResponse(int statusCode)
{
    if (_response_)
        delete _response_;

   std::string userPagePath = _request_->statusPagePath(statusCode);

	if (resourceExists(userPagePath))
		_response_ = new File(statusCode, userPagePath, *_request_);
	else
		_response_ = new DynContent(statusCode, statusPage, *_request_);
	_pollStruct_->events = POLLOUT | POLLHUP;
}

void Client::newResponse(dynCont &dynContent)
{
    if (_response_)
        delete _response_;

    _response_ = new DynContent(200, dynContent, *_request_);
    _pollStruct_->events = POLLOUT | POLLHUP;
}

void Client::handlePost()
{
    std::ofstream file;
    std::string path;

    if (_request_->cgiRequest())
        path = _request_->cgiIn();
    else
    {
        path = _request_->updatedURL();
        if (resourceExists(path) && !_request_->locationInfo()->delete_)
            throw ErrCode(409, "MYNAME"); //why MYNAME is not working btw? if delete is not allowed (=== file is there)
    }
    if (_append_)
        file.open(path.c_str(), std::ios::binary | std::ios::app);
    else
    {
        file.open(path.c_str(), std::ios::binary | std::ios::trunc);
        _append_ = true;
    }
    if (!file)
    {
        file.close();
        throw ErrCode(500, "MYNAME"); // change later;
    }
    file.write(_buffer_.c_str(), _buffer_.size());
    _bytesWritten_ += _buffer_.size();
    _buffer_.clear();
    file.close();
    if (_bytesWritten_ >= _request_->contentLength())
    {
        if (_request_->cgiRequest())
            handleCGI();
        else
            newResponse(201);
    }
}

void Client::handleDelete()
{
    if (isDirectory(_request_->updatedURL()))
		newResponse(405);
	else if (!resourceExists(_request_->updatedURL()))
		newResponse(404);
	else if (remove(_request_->updatedURL().c_str()) == 0)
		newResponse(204);
	else
		newResponse(500);
}

bool    Client::handleCGI()
{
    int status;
    if (!_cgiInProgress_)
    {
        _pollStruct_->events = POLLOUT | POLLHUP;
        launchChild();
    }
    if (waitpid(_pid_, &status, WNOHANG) == 0)
	{
		if (_time_ + CGI_TIMEOUT < time(NULL))
		{
			std::cerr << E_CL_CHILDTIMEOUT << std::endl;
			kill(_pid_, SIGKILL);
			waitpid(_pid_, &status, 0); // have to wait for child to finish dying
		}
		else
			return true; // child still running, but not timed out.
	}
	
	_cgiInProgress_ = false;

	if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) // WIFEXITED(status) == 0 -> child was interrupted
	{
		std::cerr << E_CL_CHILD << std::endl;
		throw ErrCode(500, "MYNAME"); //
	}
	
	if (resourceExists(_request_->cgiOut()))
		newResponse(_request_->cgiOut());
	else
		newResponse(500);

	if (_request_->method() == POST && remove(_request_->cgiIn().c_str()) != 0)
		std::cerr << E_CL_TEMPFILEDEL << std::endl;
	
	return false;
}

void    Client::launchChild()
{
    makeEnv();
    if ((_pid_=fork()) == -1)
    {
        perror("fork error");
        throw ErrCode(500, "MYNAME"); //
    }
    if (_pid_ == 0)
    {
        for (size_t i = 0; i < _pollStructs_.size(); ++i)
            close(_pollStructs_[i].fd);
        execve(_request_->cgiExecPath().c_str(), _argVVec_.data(), _envVec_.data());
        perror("execve error");
        exit(EXIT_FAILURE);
    }
    else
    {
        _time_ = time(NULL);
        _cgiInProgress_ = true;
    }
}

void    Client::makeEnv()
{
    _argVVecStr_.push_back(_request_->cgiExecPath());
    _argVVecStr_.push_back(_request_->updatedURL());
    for (size_t i = 0; i < _argVVec_.size(); ++i)
        _argVVec_.push_back(const_cast<char *>(_argVVecStr_[i].c_str())); //wht ?   
    _argVVec_.push_back(NULL);

    std::stringstream   contentLength, port;
    std::string     cookie, ipAddress, userAgent;
    contentLength << _request_->contentLength();
	port << ntohs(_request_->activeConfig()->getPort());

	if (_request_->headers()->find("cookie") != _request_->headers()->end())
		cookie = _request_->headers()->find("cookie")->second;

	ipAddress = inet_ntoa(_address_.sin_addr);

	if (_request_->headers()->find("user-agent") != _request_->headers()->end())
		userAgent = _request_->headers()->find("user-agent")->second;
	
	_envVecStr_.push_back("SCRIPT_NAME=" + _request_->file());
	_envVecStr_.push_back("QUERY_STRING=" + _request_->queryString());
	_envVecStr_.push_back("REQUEST_METHOD=" + _request_->method());
	_envVecStr_.push_back("CONTENT_TYPE=" + _request_->contentType());
	_envVecStr_.push_back("CONTENT_LENGTH=" + contentLength.str());
	_envVecStr_.push_back("HTTP_COOKIE=" + cookie);
	_envVecStr_.push_back("REMOTE_ADDR=" + ipAddress);
	_envVecStr_.push_back("SERVER_NAME=" + _request_->selectedHost());
	_envVecStr_.push_back("SERVER_PORT=" + port.str());
	_envVecStr_.push_back("PATH_INFO=" + _request_->updatedURL());
	_envVecStr_.push_back("HTTP_USER_AGENT=" + userAgent);
	_envVecStr_.push_back("INPUT_FILE=" + _request_->cgiIn());
	_envVecStr_.push_back("OUTPUT_FILE=" + _request_->cgiOut());

	for (size_t i = 0; i < _envVecStr_.size(); ++i)
		_envVec_.push_back(const_cast<char*>(_envVecStr_[i].c_str()));
	_envVec_.push_back(NULL);

}

void Client::sendStatusPage(int code)
{
    newResponse(code);
}

std::string Client::prependClassName(std::string function)
{
	return "Client::" + function;
}

void Client::whoIsI() const
{
	std::stringstream title;
	title << "----- Client on fd " << _fd_ << " with session id " << _request_->sessionID() << " -----";
	std::string separator(title.str().size(), '-');
	
	std::cout << "\n" << title.str() << "\n";
	_request_->whoIsI();
	std::cout << separator << "\n" << std::endl;
}
