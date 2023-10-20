#ifndef ERRCODE_HPP
#define ERRCODE_HPP

#include "../webserv.hpp"

class ErrCode{
    public:
		ErrCode(int, std::string);
		virtual ~ErrCode() throw();

		const char*	what() const throw();
		int			getCode() const;

	private:
		int				_errorCode;
		std::string		_errorMsg;
};

#endif