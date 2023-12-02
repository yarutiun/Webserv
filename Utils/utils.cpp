#include"utils.hpp"


void sigHandler(int sig)
{
    std::cout << "Signal received: " << sig << std::endl;
    signum = 1;

}

std::string strToLower(const std::string& str)
{
	std::string	lowerStr = str;

	for (std::string::iterator it = lowerStr.begin(); it != lowerStr.end(); it++)
		*it = tolower(*it);
	return lowerStr;
}

std::string splitEraseStr(std::string& input, const std::string& targetString)
{
	std::string	element;
	size_t 		len;

	len = input.find(targetString);
	if (len == std::string::npos)
	{
		element = input;
		input.erase();
	}
	else
	{
		element = input.substr(0, len);
		input.erase(0, len + targetString.size());
	}
	trimWhitespace(element);
	return element;
}

std::string trimWhitespace(std::string& input)
{
	size_t	start = input.find_first_not_of(WHITESPACE);
	size_t	end = input.find_last_not_of(WHITESPACE);
	if (start == std::string::npos)
		input = "";
	else
		input = input.substr(start, end - start + 1);
	return input;
}

std::map<std::string, std::string> parseStrMap(std::string& input, const std::string& endOfKey, const std::string& endOfValue, const std::string& endOfMap)
{
	std::map<std::string, std::string> 	stringMap;
	std::string 						key, value;

	if (endOfMap.empty())
	{
		while (!input.empty())
		{
			key = splitEraseStr(input, endOfKey);
			value = splitEraseStr(input, endOfValue);
			stringMap.insert(std::make_pair(strToLower(key), value));
		}
	}
	else // same function as before, except for the if()
	{
		while (!input.empty())
		{
			if (input.find(endOfMap) == 0)
			{
				input = input.substr(endOfMap.size());
				return stringMap;
			}
			key = splitEraseStr(input, endOfKey);
			value = splitEraseStr(input, endOfValue);
			stringMap.insert(std::make_pair(strToLower(key), value));
		}
	}

	return stringMap;
}



std::string generateSessionId()
{
	char		sessionID[17];
	const char	charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	size_t		i = 0;
	
	srand(time(NULL));
	for (; i < sizeof(sessionID) - 1; ++i)
		sessionID[i] = charset[rand() % (sizeof(charset) - 1)];
	sessionID[i] = 0;
	return sessionID;
}

std::string currentTimeCPP98()
{
	time_t		rawtime;
	const char*	timeformat = "%G-%m-%d %H:%M:%S";
	char		timeoutput[69];

	time(&rawtime);
	tm* timeinfo = localtime(&rawtime);
	strftime(timeoutput, 420, timeformat, timeinfo);
	std::string returnVal(timeoutput);
	return returnVal;
}

bool isStringInVec(const std::string& string, const std::vector<std::string>& vector)
{
	if (std::find(vector.begin(), vector.end(), string) != vector.end())
		return true;
	return false;
}

std::string fileExtension(const std::string& filename)
{
	size_t dotPosition = filename.find_last_of(".");
	if (dotPosition == std::string::npos)
		return "";
	return filename.substr(dotPosition);
}

bool isDirectory(const std::string& path)
{
	struct stat	fileInfo;
	
	if (stat(path.c_str(), &fileInfo) == 0)
		return S_ISDIR(fileInfo.st_mode);
	return false;
}

std::string getHttpMsg(int code)
{
	switch (code)
	{
		case 0: return "No error.";
		
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Checkpoint";

		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";

		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Switch Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";

		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Payload Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Entity";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Request";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable For Legal Reasons";

		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";

		default: return "Unknown Error";
	}
}

bool resourceExists(const std::string& path)
{
	if (path.empty())
		return false;
	
	std::ifstream	resource(path.c_str());
	bool			exists = resource.good();
	
	resource.close();
	return exists;
}

std::string splitEraseTrimChars(std::string& input, const std::string& targetChars)
{
    std::string element;
    size_t 		len;

    len = input.find_first_of(targetChars);
    if (len == std::string::npos)
    {
        element = input;
        input.erase();
    }
    else
    {
        element = input.substr(0, len);
        input.erase(0, len);
        trimWhitespace(input);
    }
    trimWhitespace(element);
    return element;
}

bool isAlnumStrPlus(const std::string& input, const std::string& allowedChars)
{
    for (std::string::const_iterator it = input.begin(); it != input.end(); it++)
        if (!isalnum(*it) && allowedChars.find(*it) == std::string::npos)
            return false;
    return true;
}

std::vector<std::string> splitEraseStrVec(std::string& input, const std::string& targetChars, const std::string& endOfParsing)
{
    std::vector<std::string>	stringVector;
    std::string					parseRegion, element;

    parseRegion = splitEraseTrimChars(input, endOfParsing);
    input.erase(0, endOfParsing.size());
    while (!parseRegion.empty())
    {
        element = splitEraseTrimChars(parseRegion, targetChars);
        stringVector.push_back(element);
    }
    return stringVector;
}

std::string getInstruction(std::string& inputStr)
{
    std::string	instruction;
    size_t		len_semicolon;

    // Check whether semicolon delimits the instruction
    len_semicolon = inputStr.find(";");
    if (len_semicolon < inputStr.find("{"))
    {
        instruction = inputStr.substr(0, len_semicolon);
        inputStr.erase(0, len_semicolon + 1);
        trimWhitespace(instruction);
        return instruction;
    }

    // Curly braces must now delimit the instruction
    size_t	i;
    int		bracesDepth;

    i = inputStr.find("{");
    if (i == std::string::npos || inputStr.find("}") < i)
        throw std::runtime_error(E_INVALIDENDTOKEN + inputStr + '\n');
    bracesDepth = 1;
    while (inputStr[++i] && bracesDepth > 0 && bracesDepth < 3)
    {
        if (inputStr[i] == '{')
            bracesDepth++;
        else if (inputStr[i] == '}')
            bracesDepth--;
    }
    if (bracesDepth != 0)
        throw std::runtime_error(E_INVALIDBRACE + inputStr + '\n');
    instruction = inputStr.substr(0, i);
    inputStr.erase(0, i);
    instruction.replace(instruction.find("{"), 1, " ");
    instruction.replace(instruction.find_last_of("}"), 1, " ");
    trimWhitespace(instruction);
    return instruction;
}

size_t fileSize(const std::string& filePath)
{
	struct stat	fileInfo;

	if (filePath.empty())
		return 0;
	if (stat(filePath.c_str(), &fileInfo) != 0)
		return 0;
	return static_cast<size_t>(fileInfo.st_size);
}

std::string getMimeType(const std::string& filePath) 
{
	std::string	extension = fileExtension(filePath);

	if (mimeTypes.find(extension) == mimeTypes.end())
		return "application/octet-stream";
	return mimeTypes[extension];
}

std::map<std::string, std::string> initMimeTypesMap()
{
	std::map<std::string, std::string>	mimeTypes;

	mimeTypes[".html"] = "text/html";
	mimeTypes[".htm"] = "text/html";
	mimeTypes[".css"] = "text/css";
	mimeTypes[".js"] = "application/javascript";
	mimeTypes[".json"] = "application/json";
	mimeTypes[".jpg"] = "image/jpeg";
	mimeTypes[".jpeg"] = "image/jpeg";
	mimeTypes[".png"] = "image/png";
	mimeTypes[".gif"] = "image/gif";
	mimeTypes[".bmp"] = "image/bmp";
	mimeTypes[".ico"] = "image/x-icon";
	mimeTypes[".svg"] = "image/svg+xml";
	mimeTypes[".xml"] = "application/xml";
	mimeTypes[".pdf"] = "application/pdf";
	mimeTypes[".zip"] = "application/zip";
	mimeTypes[".gz"] = "application/gzip";
	mimeTypes[".tar"] = "application/x-tar";
	mimeTypes[".mp4"] = "video/mp4";
	mimeTypes[".mpeg"] = "video/mpeg";
	mimeTypes[".avi"] = "video/x-msvideo";
	mimeTypes[".avif"] = "image/avif";
	mimeTypes[".ogg"] = "audio/ogg";
	mimeTypes[".mp3"] = "audio/mpeg";
	mimeTypes[".wav"] = "audio/wav";
	mimeTypes[".mov"] = "video/quicktime";
	mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
	mimeTypes[".xls"] = "application/vnd.ms-excel";
	mimeTypes[".doc"] = "application/msword";
	mimeTypes[".csv"] = "text/csv";
	mimeTypes[".txt"] = "text/plain";
	mimeTypes[".rtf"] = "application/rtf";
	mimeTypes[".shtml"] = "text/html";
	mimeTypes[".php"] = "application/php";
	mimeTypes[".jsp"] = "text/plain";
	mimeTypes[".swf"] = "application/x-shockwave-flash";
	mimeTypes[".ttf"] = "application/x-font-truetype";
	mimeTypes[".eot"] = "application/vnd.ms-fontobject";
	mimeTypes[".woff"] = "application/font-woff";
	mimeTypes[".woff2"] = "font/woff2";
	mimeTypes[".ics"] = "text/calendar";
	mimeTypes[".vcf"] = "text/x-vcard";
	mimeTypes[".mid"] = "audio/midi";
	mimeTypes[".midi"] = "audio/midi";
	mimeTypes[".wmv"] = "video/x-ms-wmv";
	mimeTypes[".webm"] = "video/webm";
	mimeTypes[".3gp"] = "video/3gpp";
	mimeTypes[".3g2"] = "video/3gpp2";
	mimeTypes[".pl"] = "text/plain";
	mimeTypes[".py"] = "text/x-python";
	mimeTypes[".java"] = "text/x-java-source";
	mimeTypes[".c"] = "text/x-c";
	mimeTypes[".cpp"] = "text/x-c++";
	mimeTypes[".cs"] = "text/plain";
	mimeTypes[".rb"] = "text/x-ruby";
	mimeTypes[".htm"] = "text/html";
	mimeTypes[".shtml"] = "text/html";
	mimeTypes[".xhtml"] = "application/xhtml+xml";
	mimeTypes[".m4a"] = "audio/mp4";
	mimeTypes[".mp4a"] = "audio/mp4";
	mimeTypes[".oga"] = "audio/ogg";
	mimeTypes[".ogv"] = "video/ogg";
	mimeTypes[".ogx"] = "application/ogg";
	mimeTypes[".oga"] = "audio/ogg";
	mimeTypes[".m3u8"] = "application/vnd.apple.mpegurl";
	mimeTypes[".qt"] = "video/quicktime";
	mimeTypes[".ts"] = "video/mp2t";
	mimeTypes[".xl"] = "application/excel";
	mimeTypes[".cab"] = "application/vnd.ms-cab-compressed";
	mimeTypes[".msi"] = "application/x-msdownload";
	mimeTypes[".dmg"] = "application/x-apple-diskimage";
	mimeTypes[".exe"] = "application/octet-stream";
	mimeTypes[".bin"] = "application/octet-stream";
	mimeTypes[".ps"] = "application/postscript";
	mimeTypes[".so"] = "application/octet-stream";
	mimeTypes[".dll"] = "application/octet-stream";
	mimeTypes[".m4v"] = "video/x-m4v";
	mimeTypes[".ser"] = "application/java-serialized-object";
	mimeTypes[".sh"] = "application/x-sh";
	mimeTypes[".log"] = "text/plain";
	mimeTypes[".diff"] = "text/x-diff";
	mimeTypes[".patch"] = "text/x-diff";
	mimeTypes[".xhtml"] = "application/xhtml+xml";
	mimeTypes[".php"] = "application/x-httpd-php";
	mimeTypes[".plist"] = "application/xml";
	mimeTypes[".sln"] = "text/plain";
	mimeTypes[".tiff"] = "image/tiff";
	mimeTypes[".app"] = "application/octet-stream";
	mimeTypes[".ics"] = "text/calendar";
	mimeTypes[".webp"] = "image/webp";

	return mimeTypes;
}