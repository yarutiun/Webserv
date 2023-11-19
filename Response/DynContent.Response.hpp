#ifndef DYNCONTENT_RESPONSE_HPP
# define DYNCONTENT_RESPONSE_HPP

# include "../webserv.hpp"

class DynContent: public Response
{
	public:
		DynContent(int, dynCont, const Request&);
		DynContent(const DynContent&);
		~DynContent() {}

		bool				send(int);
		Response*			clone() const;
	
	private:
		std::string			buildSessionLogPage();
		std::string			buildDirListingPage();
		std::string			buildStatusPage();
};

#endif
