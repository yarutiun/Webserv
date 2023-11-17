#ifndef CONFIGURATION_H
# define CONFIGURATION_H

# include "../webserv.hpp"

class Configuration {

private:
    std::vector<std::string>								        _names;
    in_addr_t												        _host;
    in_port_t												        _port;
    std::string												        _root;
    bool													        _defaultDirListing;
    unsigned long											        _clientMaxBody;
    size_t													        _maxConnections;
    std::string												        _standardFile;
    std::map<int, std::string>								        _statusPagePaths;
    std::map<std::string, locInfo>							        _locations;
    std::map<std::string, std::string>						        _cgiPaths;
    std::vector<Configuration>										_altConfigs;
    std::map<std::string, void(Configuration::*)(std::string&)>	    _functionMap;

    void										                    _setFunctionMap();
    void										                    _parseNames(std::string &input);
    void										                    _parseHost(std::string &input);
    void										                    _parsePort(std::string &input);
    void										                    _parseRoot(std::string &input);
    void										                    _parseDirListing(std::string &input);
    void										                    _parseClientMaxBody(std::string &input);
    void										                    _parseMaxConnections(std::string &input);
    void										                    _parseStandardFile(std::string &input);
    void										                    _parseStatusPagePaths(std::string &userErrorPages);
    void										                    _parseLocations(std::string &locationElement);
    void										                    _parseCgiPaths(std::string &cgiElement);

public:
    Configuration(std::string configElement);

    std::vector<std::string> const                                  &getNames() const;
    in_addr_t                                                       getHost() const;
    in_port_t                                                       getPort() const;
    std::string const                                               &getRoot() const;
    bool                                                            isDefaultDirListing() const;
    unsigned long                                                   getClientMaxBody() const;
    size_t                                                          getMaxConnections() const;
    std::string const                                               &getStandardFile() const;
    std::map<int, std::string> const                                *getStatusPagePaths() const;
    std::map<std::string, locInfo> const                            *getLocations() const;
    std::map<std::string, std::string> const                        *getCgiPaths() const;
    std::vector<Configuration> const                                &getAltConfigs() const;

    void										                    whoIsI() const;
    void										                    addAltConfig(const Configuration &altConfig);
};

#endif

