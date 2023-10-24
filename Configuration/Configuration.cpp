#include "../webserv.hpp"

//Private
void Configuration::_setFunctionMap()
{
    _functionMap[SERVERNAME] = &Configuration::_parseNames;
    _functionMap[HOST] = &Configuration::_parseHost;
    _functionMap[PORT] = &Configuration::_parsePort;
    _functionMap[ROOT] = &Configuration::_parseRoot;
    _functionMap[DIRLISTING] = &Configuration::_parseDirListing;
    _functionMap[CLIMAXBODY] = &Configuration::_parseClientMaxBody;
    _functionMap[MAXCONNS] = &Configuration::_parseMaxConnections;
    _functionMap[STDFILE] = &Configuration::_parseStandardFile;
    _functionMap[ERRORPAGETITLE] = &Configuration::_parseStatusPagePaths;
    _functionMap[LOCATIONTITLE] = &Configuration::_parseLocations;
    _functionMap[CGITITLE] = &Configuration::_parseCgiPaths;
}

void Configuration::_parseNames(std::string& input)
{
    std::string name;

    while (!input.empty())
    {
        name = splitEraseTrimChars(input, WHITESPACE);
        if (isAlnumStrPlus(name, "._-/"))
            _names.push_back(name);
        else
            std::cerr << I_C_INVALSERVNAME << name << std::endl;
    }
    if (_names.empty())
        _names.push_back("default");
}

void Configuration::_parseHost(std::string& input)
{
    if (input == "ANY")
        _host = INADDR_ANY;
    else
    {
        _host = inet_addr(input.c_str());
        if (_host == INADDR_NONE)
            throw std::runtime_error(E_C_HOSTADDRVAL + input + '\n');
    }
}

void Configuration::_parsePort(std::string& input)
{
    if (input.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error(E_C_PORTINPUT + input + '\n');

    _port = (uint16_t)atoi(input.c_str());

    if (_port > (uint16_t)65534)
        throw std::runtime_error(E_C_PORTVAL + input + '\n');

    _port = htons(_port);

}

void Configuration::_parseRoot(std::string& input)
{
    if (!isAlnumStrPlus(input, "._-/"))
        throw std::runtime_error(E_C_ROOTINPUT + input + '\n');

    _root = input;

    if (*(_root.end() - 1) != '/')
        std::cerr << I_C_NONDIRPATH << _root << std::endl;
}

void Configuration::_parseDirListing(std::string& input)
{
    if (input == "yes")
        _defaultDirListing = true;
    else
    {
        _defaultDirListing = false;
        if (input != "no")
            std::cerr << I_C_INVALIDVALUE << input << std::endl;
    }
}

void Configuration::_parseClientMaxBody(std::string& input)
{
    if (input.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error(E_C_MAXCLBODINPUT + input + '\n');

    std::istringstream	iss(input);

    iss >> _clientMaxBody;

    if (_clientMaxBody > (unsigned long)MAX_MAXCLIENTBODY)
        throw std::runtime_error(E_C_MAXCLBODHIGH + input + '\n');
}

void Configuration::_parseMaxConnections(std::string& input)
{
    if (input.find_first_not_of("0123456789") != std::string::npos)
        throw std::runtime_error(E_C_MAXCONNINPUT + input + '\n');

    _maxConnections = atoi(input.c_str());

    if (_maxConnections > MAX_MAXCONNECTIONS)
        throw std::runtime_error(E_C_MAXCONNVAL + input + '\n');
}

void Configuration::_parseStandardFile(std::string& input)
{
    if (!isAlnumStrPlus(input, "._-/"))
        throw std::runtime_error(E_C_STDFILEINPUT + input + '\n');

    _standardFile = input;
}

void Configuration::_parseStatusPagePaths(std::string& userErrorPages)
{
    std::string					key, value;
    std::vector<std::string>	lineStrings;
    int							code;

    while (!userErrorPages.empty())
    {
        lineStrings = splitEraseStrVec(userErrorPages, WHITESPACE, ";");
        if (lineStrings.size() < 2)
        {
            std::cerr << I_C_MISSINGVAL << lineStrings[0] << std::endl;
            continue;
        }
        value = lineStrings.back();
        lineStrings.pop_back();
        while (!lineStrings.empty())
        {
            key = lineStrings.back();
            code = atoi(key.c_str());
            if (_statusPagePaths.find(code) != _statusPagePaths.end())
                _statusPagePaths[code] = value;
            else if (code > 99 && code < 600)
                _statusPagePaths.insert(std::make_pair(code, value));
            else
                std::cerr << I_C_INVALSTATPAGE << key << std::endl;
            lineStrings.pop_back();
        }
    }
}

void Configuration::_parseLocations(std::string& locationElement)
{
    std::string									path, instruction, key;
    std::map<std::string, locInfo>::iterator	iter;
    locInfo									locInfo;

    path = splitEraseTrimChars(locationElement, WHITESPACE);

    if (*(path.end() - 1) != '/')
        std::cerr << I_C_NONDIRPATH << path << std::endl;

    while (!locationElement.empty())
    {
        instruction = getInstruction(locationElement);
        key = splitEraseTrimChars(instruction, WHITESPACE);
        if (key == METHODS)
        {
            if (instruction.find(GET) != std::string::npos)
                locInfo.get = true;
            else
                locInfo.get = false;
            if (instruction.find(POST) != std::string::npos)
                locInfo.post = true;
            else
                locInfo.post = false;
            if (instruction.find(DELETE) != std::string::npos)
                locInfo.delete_ = true;
            else
                locInfo.delete_ = false;
        }
        else if (key == DIRLISTING)
        {
            if (instruction == "yes" || instruction == "no")
                locInfo.dir_listing = instruction;
            else
                std::cerr << I_C_INVALIDVALUE << instruction << std::endl;
        }
        else if (key == REDIRECTION)
            locInfo.http_redir = instruction;
        else if (key == UPLOADDIR)
            locInfo.upload_dir = instruction;
        else if (key == STDFILE)
            locInfo.std_file = instruction;
        else
            std::cerr << I_C_INVALIDKEY << key << std::endl;
    }
    _locations.insert(std::make_pair(path, locInfo));
}

void Configuration::_parseCgiPaths(std::string& cgiElement)
{
    std::string										instruction, key;
    std::map<std::string, std::string>::iterator	iter;

    while (!cgiElement.empty())
    {
        instruction = getInstruction(cgiElement);
        key = splitEraseTrimChars(instruction, WHITESPACE);
        if (instruction.empty())
        {
            std::cerr << I_C_MISSINGVAL << key << std::endl;
            continue;
        }
        iter = _cgiPaths.find(key);
        if (iter != _cgiPaths.end())
            iter->second = instruction;
        else
            _cgiPaths.insert(std::make_pair(key, instruction));
    }
}

//Public
Configuration::Configuration(std::string configStr)
{
    std::string	instruction, key;

    _setFunctionMap();

    while (!configStr.empty())
    {
        instruction = getInstruction(configStr);
        key = splitEraseTrimChars(instruction, " \t\v\r\n{");

        std::map<std::string, void(Configuration::*)(std::string&)>::iterator it = _functionMap.find(key);

        if (it != _functionMap.end())
            (*this.*(it->second))(instruction);
        else
        {
            std::cerr << I_C_INVALIDKEY << key << std::endl;
            continue;
        }
    }
}

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

std::vector<Configuration> const &Configuration::getAltConfigs() const {
    return _altConfigs;
}

void Configuration::whoIsI() const
{
    std::cout	<< "\n";
    if (!_names.empty())
    {
        std::cout << "name(s):\t\t" << *_names.begin() << '\n';
        for (std::vector<std::string>::const_iterator it = ++_names.begin(); it != _names.end(); ++it)
            std::cout << "\t\t\t" << *it << '\n';
    }
    std::cout   << "host:\t\t\t" << _host << '\n'
                << "port:\t\t\t" << ntohs(_port) << '\n'
                << "root:\t\t\t" << _root << '\n'
                << "default dirListing:\t" << (_defaultDirListing ? "yes" : "no") << '\n'
                << "client max body:\t" << _clientMaxBody << '\n'
                << "max connections:\t" << _maxConnections << '\n'
                << "standardfile:\t\t" << _standardFile << '\n';
    if (!_statusPagePaths.empty())
    {
        std::cout << "error Pages:\t\t" << _statusPagePaths.begin()->first << '\t' << _statusPagePaths.begin()->second << '\n';
        for (std::map<int, std::string>::const_iterator it = ++_statusPagePaths.begin(); it != _statusPagePaths.end(); it++)
            std::cout << "\t\t\t" << it->first << '\t' << it->second << '\n';
    }
    if (!_locations.empty())
    {
        std::cout	<< "known locations:\t" << _locations.begin()->first << '\n';
        for (std::map<std::string, locInfo>::const_iterator it = ++_locations.begin(); it != _locations.end(); it++)
            std::cout << "\t\t\t" << it->first << '\n';
    }
    if (!_cgiPaths.empty())
    {
        std::cout	<< "CGI paths:\t\t" << _cgiPaths.begin()->first << '\t' << _cgiPaths.begin()->second << '\n';
        for (std::map<std::string, std::string>::const_iterator it = ++_cgiPaths.begin(); it != _cgiPaths.end(); it++)
            std::cout << "\t\t\t" << it->first << '\t' << it->second << std::endl;
    }
}

void Configuration::addAltConfig(const Configuration &altConfig)
{
    _altConfigs.push_back(altConfig);
}
