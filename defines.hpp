#define RECV_CHUNK_SIZE 16384 //why?
# define WHITESPACE		" \t\v\r\n"
# define MAX_REQHEADSIZE		8192
# define SESSIONID		"sessionid"
# define SYS_LOGS			"system/logs/"
# define HTTPVERSION	"HTTP/1.1"
# define MYNAME			prependClassName(__FUNCTION__)


// configuration element identifiers
# define SERVER				"server"
# define SERVERNAME			"server_name"
# define HOST				"host"
# define PORT				"listen"
# define ROOT				"root"
# define CLIMAXBODY			"client_max_body"
# define BACKLOG			"backlog"
# define MAXCONNS			"max_connections"
# define STDFILE			"standard_file"
# define UPLOADDIR			"upload_dir"
# define ERRORPAGETITLE		"error_pages"
# define ERRPAGEDEFAULT		"default"
# define LOCATIONTITLE		"location"
# define DIRLISTING			"dir_listing"
# define REDIRECTION		"http_redirect"
# define METHODS			"methods"
# define GET				"GET"
# define POST				"POST"
# define DELETE				"DELETE"
# define CGITITLE			"cgi"
# define SYS_TEMP_CGIIN		"system/temp/cgiIn_fd"
# define SYS_TEMP_CGIOUT	"system/temp/cgiOut_fd"
# define SYS_LOGS			"system/logs/"
