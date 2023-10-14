#include "webserv.hpp"

std::vector<std::string> const &Configuration::getNames() const {
    return _names;
}

in_addr_t Configuration::getHost() const {
    return _host;
}

in_port_t Configuration::getPort() const {
    return _port;
}

std::string const &Configuration::getRoot() const {
    return _root;
}

bool Configuration::isDefaultDirListing() const {
    return _defaultDirListing;
}

unsigned long Configuration::getClientMaxBody() const {
    return _clientMaxBody;
}

size_t Configuration::getMaxConnections() const {
    return _maxConnections;
}

std::string const &Configuration::getStandardFile() const {
    return _standardFile;
}

std::map<int, std::string> const &Configuration::getStatusPagePaths() const {
    return _statusPagePaths;
}

std::map<std::string, locInfo> const &Configuration::getLocations() const {
    return _locations;
}

std::map<std::string, std::string> const &Configuration::getCgiPaths() const {
    return _cgiPaths;
}

std::vector<Config> const &Configuration::getAltConfigs() const {
    return _altConfigs;
}
