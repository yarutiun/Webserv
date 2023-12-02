#include "../webserv.hpp"

File::File(int code, std::string sendPath, const Request& request):
	Response(request),
	_sendPath(sendPath),
	_responseHeadIncomplete(true),
	_filePosition(0)
{
	_code = code;
	_contentLength = fileSize(sendPath);
	_contentType = getMimeType(sendPath);
	_sendBuffer	<< buildResponseHead();
}

File::File(const File& src):
	Response(src),
	_sendPath(src._sendPath),
	_responseHeadIncomplete(src._responseHeadIncomplete),
	_filePosition(src._filePosition)
{}

Response* File::clone() const
{
	return new File(*this);
}

bool File::send(int fd)
{
	if (_responseHeadIncomplete)
	{
		whoIsI(fd);
		_responseHeadIncomplete = sendInternalBuffer(fd);
		return true;
	}

	std::ifstream	fileStream(_sendPath.c_str(), std::ios::binary);
	
	if (!fileStream)
	{
		fileStream.close();
		throw ErrCode(500, __FUNCTION__);
	}
	
	char	buffer[SEND_CHUNK_SIZE];
	
	fileStream.seekg(_filePosition);
	fileStream.read(buffer, SEND_CHUNK_SIZE);

	if (::send(fd, buffer, fileStream.gcount(), 0) == -1)
	{
		fileStream.close();
		throw CloseConnection(__FUNCTION__, E_SEND);
	}
	
	if (fileStream.eof())
	{
		fileStream.close();
		if (_request.cgiRequest())
		{
			if (unlink(_sendPath.c_str()) != 0)
			std::cerr << E_TEMPFILEREMOVAL << std::endl;
		}
		return false;
	}
	
	_filePosition = fileStream.tellg();
	fileStream.close();
	return true;
}
