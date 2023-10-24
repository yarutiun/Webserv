#ifndef REQUEST_HPP
#define REQUEST_HPP


#include "../webserv.hpp"

class Request
{
	public:
		Request(std::string&, const Configuration&, const Client&);
		// Request(std::string&, const Configuration &,const Client&);  // delete later
		Request(const Request&);
		
		Request& 									operator=(const Request&);
		void										whoIsI() const;
		void										process();

		const Configuration*								activeConfig() const;
		const std::string&							method() const;
		const std::string&							URL() const;
		const std::string&							httpProt() const;
		const std::string&							queryString() const;
		const std::map<std::string, std::string>*	headers() const;
		const std::map<std::string, std::string>*	cookies() const;
		const std::string&							requestedHost() const;
		const std::string&							selectedHost() const;
		size_t										contentLength() const;
		const std::string&							contentType() const;
		const std::string&							directory() const;
		const std::string&							file() const;
		bool										dirListing() const;
		bool										cgiRequest() const;
		const std::string&							cgiExecPath() const;
		const std::string&							cgiIn() const;
		const std::string&							cgiOut() const;
		const dynCont&								internalScript() const;

		bool										setCookie() const;
		const std::string&							sessionID() const;
		const std::string&							standardFile() const;
		const std::string&							updatedDir() const;
		const std::string&							updatedURL() const;
		const locInfo*								locationInfo() const;
		std::string									statusPagePath(int) const;
		const std::string&							root() const;
	
	private:
		std::string									prependClassName(std::string);
		void										parseRequestLine();
		void										parseRequestHeaders();
		void										handleSessionID();
		void										trackSession();
		void										selectConfig();
		void										requestError();
		void										updateVars();
		std::string									prependRoot(const std::string&) const;
		std::string 								appendSlash(const std::string&);

		std::string*								buffer;
		const Configuration*								_config;
		const Configuration*								_activeConfig;
		const Client*								_client;
		std::string									_method;
		std::string									_URL;
		std::string									_httpProtocol;
		std::string									_queryString;
		std::map<std::string, std::string>			_headers;
		std::map<std::string, std::string>			_cookies;
		std::string									_requestedHost;
		std::string									_selectedHost;
		unsigned long								_contentLength;
		std::string									_contentType;
		std::string									_sessionID;
		std::string									_directory;
		std::string									_file;
		bool										_cgiRequest;
		std::string									_cgiExecPath;
		std::string									_cgiIn;
		std::string									_cgiOut;
		dynCont										_internalScript;
		bool										_setCookie;
		std::string									_standardFile;
		std::string									_updatedDirectory;
		std::string									_updatedURL;
		locInfo										_locationInfo;
};

#endif 