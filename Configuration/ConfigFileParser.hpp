#ifndef CONFIGFILEPARSER_HPP
#define CONFIGFILEPARSER_HPP

#include "../webserv.hpp"

class ConfigFileParser {
private:
    std::vector<Configuration>     _configs_;

    std::string                     _loadFile(const char *file);
    std::string                     _getConfigElement(std::string &element);
    bool                            _combineSharedNetAddress(const Configuration &config, size_t size);
    bool                            _sharedNetAddress(const Configuration &configOne, const Configuration &configTwo);

public:
    ConfigFileParser(const char *configFile);

    std::vector<Configuration>     getConfigs() const;
};

#endif