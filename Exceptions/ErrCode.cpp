#include "../webserv.hpp"

ErrCode::ErrCode(int errorCode, std::string throwingFunction):
	_errorCode(errorCode)
{
	std::stringstream ss;
	
	ss << "\n" << throwingFunction << ": " << errorCode << " " << getHttpMsg(errorCode);
	_errorMsg = ss.str();
}

ErrCode::~ErrCode() throw() {}

const char* ErrCode::what() const throw() { return _errorMsg.c_str(); }

int ErrCode::getCode() const { return _errorCode; }