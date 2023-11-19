#ifndef FILE_RESPONSE_HPP
# define FILE_RESPONSE_HPP

# include "../webserv.hpp"

class File: public Response
{
	public:
		File(int, std::string, const Request&);
		File(const File&);
		~File() {}

		bool				send(int);
		Response*			clone() const;
	
	private:
		std::string			_sendPath;
		bool				_responseHeadIncomplete;
		std::streampos		_filePosition;
};

#endif
