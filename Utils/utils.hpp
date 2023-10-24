#ifndef UTILS_HPP
#define UTILS_HPP

#include"../webserv.hpp"

void sigHandler(int signum);
std::string splitEraseStr(std::string &, const std::string&);
std::string trimWhitespace(std::string& input);
std::map<std::string, std::string> parseStrMap(std::string& input, const std::string& endOfKey, const std::string& endOfValue, const std::string& endOfMap);
std::string generateSessionId();
std::string currentTimeCPP98();
bool isStringInVec(const std::string& string, const std::vector<std::string>& vector);
std::string fileExtension(const std::string& filename);
bool isDirectory(const std::string& path);
std::string getHttpMsg(int code);
bool resourceExists(const std::string& path);
std::string	splitEraseTrimChars(std::string&, const std::string&);
bool isAlnumStrPlus(const std::string&, const std::string&);
std::vector<std::string> splitEraseStrVec(std::string&, const std::string&, const std::string&);
std::string getInstruction(std::string&);
size_t fileSize(const std::string& filePath);
std::string getMimeType(const std::string& filePath);
std::map<std::string, std::string> initMimeTypesMap();


#endif