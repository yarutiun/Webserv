#ifndef CLOSECONNECTION_EXCEPTION_HPP
# define CLOSECONNECTION_EXCEPTION_HPP

# include "../webserv.hpp"

class CloseConnection: public std::exception
{
	public:
		CloseConnection(std::string, std::string);
		virtual ~CloseConnection() throw();

		const char*	what() const throw();

	private:
		std::string		_errorMsg;
};

#endif