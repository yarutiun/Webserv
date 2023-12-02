#include"../webserv.hpp"

Response::Response(const Request& request):
	_request(request),
	_code(0),
	_sendBufPos(0)
{}

Response::Response(const Response& src):
	_request(src._request)
{
	*this = src;
}

Response& Response::operator=(const Response& src)
{
	_code = src._code;
	_contentLength = src._contentLength;
	_contentType = src._contentType;
	_sendBuffer.str(src._sendBuffer.str());
	_sendBufPos = src._sendBufPos;
	_sendBuffer.seekg(_sendBufPos);

	return *this;
}

void Response::whoIsI(int fd)
{
	std::string			responseHead = _sendBuffer.str().substr(0, _sendBuffer.str().find("\r\n\r\n"));
	std::stringstream	ss;

	ss << "---------- Response-head for Client on fd " << fd << " ----------";
	std::string	separator(ss.str().size(), '-');

	std::cout	<< "\n" << ss.str() << "\n"
				<< responseHead << "\n"
				<< separator << std::endl;
}

bool Response::sendInternalBuffer(int fd)
{
	char	buffer[SEND_CHUNK_SIZE];
	
	_sendBuffer.seekg(_sendBufPos); // need this for copy constructor: tellg() is non const and so the streampos cannot be copied
	_sendBuffer.read(buffer, SEND_CHUNK_SIZE);
	_sendBufPos = _sendBuffer.tellg();
	
	if (::send(fd, buffer, _sendBuffer.gcount(), 0) <= 0)
		throw CloseConnection(__FUNCTION__, E_SEND);
	
	if (_sendBuffer.tellg() == std::streampos(-1)) // end of buffer reached
		return false;
	return true;
}

/*
if (_request.cgiRequest())
		ss	<< "transfer-encoding: chunked\r\n"
			<< "content-type: " << getMimeType(".html") << "\r\n"; // CGI only returns html
*/
std::string Response::buildResponseHead()
{
	std::stringstream	ss;
	
	ss	<< HTTPVERSION << ' ' << _code << ' ' << getHttpMsg(_code) << "\r\n"
		<< "Server: " << SERVERVERSION << "\r\n"
		<< "connection: close" << "\r\n"
		<< "content-length: " << _contentLength << "\r\n"
		<< "content-type: " << _contentType << "\r\n";
	if (_request.setCookie())
		ss << buildCookie(SESSIONID, _request.sessionID(), 3600, "/") << "\r\n";
	ss << "\r\n";
	return ss.str();
}

std::string Response::buildCookie(const std::string& key, const std::string& value, int expiration, const std::string& path)
{
	std::stringstream cookie;

	cookie << "set-cookie: " << key << "=" << value << ";";
	if (expiration >= 0)
		cookie << "max-age=" << expiration << ";";
	cookie << "path=" << path << ";";
	return cookie.str();
}
