#ifndef CONFIGURATION_H
# define CONFIGURATION_H

# include "../webserv.hpp"

class Configuration {

private:
    std::vector<std::string>								_names;
    in_addr_t												_host;
    in_port_t												_port;
    std::string												_root;
    bool													_defaultDirListing;
    unsigned long											_clientMaxBody;
    size_t													_maxConnections;
    std::string												_standardFile;
    std::map<int, std::string>								_statusPagePaths;
    std::map<std::string, locInfo>							_locations;
    std::map<std::string, std::string>						_cgiPaths;
    std::vector<Config>										_altConfigs;
    std::map<std::string, void(Config::*)(std::string&)>	_functionMap;

public:
    std::vector<std::string> const&                         getNames() const;
    in_addr_t                                               getHost() const;
    in_port_t                                               getPort() const;
    std::string const&                                      getRoot() const;
    bool                                                    isDefaultDirListing() const;
    unsigned long                                           getClientMaxBody() const;
    size_t                                                  getMaxConnections() const;
    std::string const&                                      getStandardFile() const;
    std::map<int, std::string> const&                       getStatusPagePaths() const;
    std::map<std::string, locInfo> const&                   getLocations() const;
    std::map<std::string, std::string> const&               getCgiPaths() const;
    std::vector<Config> const&                              getAltConfigs() const;
};

#endif

