#include"utils.hpp"


void sigHandler(int sig)
{
    std::cout << "Signal received: " << sig << std::endl;
    signum = 1;

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

std::string strToLower(const std::string& str)
{
	std::string	lowerStr = str;

	for (std::string::iterator it = lowerStr.begin(); it != lowerStr.end(); it++)
		*it = tolower(*it);
	return lowerStr;
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
