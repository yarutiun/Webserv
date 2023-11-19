#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../webserv.hpp"

class File;

class Response
{
	public:
		virtual ~Response() {};

		virtual Response*	clone() const = 0; // when overriding these in derived class, should use "override" even if not necessary. But not cpp98.
		virtual bool		send(int) = 0;
	
	protected:
		Response(const Request&);
		Response(const Response&);
		
		Response&			operator=(const Response&);
		void				whoIsI(int);
		bool				sendInternalBuffer(int);
		std::string			buildResponseHead();
		std::string			buildCookie(const std::string&, const std::string&, int, const std::string&);
		
		const Request       &_request;
		int					_code;
		size_t				_contentLength;
		std::string			_contentType;
		std::stringstream	_sendBuffer;
		std::streampos		_sendBufPos;
};

#endif
