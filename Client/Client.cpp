#include "Client.hpp"


Client::Client(const Configuration &config, std::vector<struct pollfd>::iterator pollstruct, int fd, sockaddr_in address): _config_(config), _pollStruct_(pollstruct), _fd_(fd), _address_(address)
{
    std::cout << "Client created" << _fd_ << _address_.sin_addr.s_addr << std::endl;
}

int Client::getFd() const
{
    return(_fd_);
}

void Client::incomingData(std::vector<struct pollfd>::iterator pollstruct)
{
    _pollStruct_ = pollstruct;
    receive();
    // std::cout << "Client " << _fd_ << " says: " << _buffer_ << std::endl;
    if (!_request_)
        newRequest();
    //
    if ( _request_->method() == GET)
        handleGet();
    if (_request_->method() == POST)
        handlePost();
}

void Client::receive()
{
    char buff[RECV_CHUNK_SIZE];

    int bytesReceived = recv(_fd_, buff, RECV_CHUNK_SIZE, 0);
    if (bytesReceived <= 0)
        perror("recv error"); // 
    _buffer_.append(buff, bytesReceived);
}

bool Client::outgoingData()
{
    ////////
    return true; // should be amended 
}

void Client::newRequest()
{
    _request_ = new Request(_buffer_, _config_, *this); // change later
    _request_->process();
}

const char *Client::getAddr() const
{
    return(inet_ntoa(_address_.sin_addr));
}

void Client::handleGet()
{
    //if for cgi here

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


void Client::newResponse(std::string const &path)
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
        // if (_request_->cgiRequest())
        //     handleCGI();
        // else
            newResponse(201);
    }


    
}