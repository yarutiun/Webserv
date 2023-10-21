#include "../webserv.hpp"

//Private
std::string ConfigFileParser::_loadFile(const char* path)
{
    std::ifstream		infile(path);
    std::stringstream	buffer;
    std::string			line;

    if (!infile)
    {
        std::string invalidpath(path);
        throw std::runtime_error(E_CF_FILEOPEN + invalidpath + '\n');
    }

    while (std::getline(infile, line))
    {
        size_t firstParseChar = line.find_first_not_of(WHITESPACE);
        if (firstParseChar == std::string::npos || line[firstParseChar] == '#')
            continue;
        buffer << line;
    }

    infile.close();
    line = buffer.str();
    trimWhitespace(line);
    return line;
}


std::string ConfigFileParser::_getConfigElement(std::string& configData)
{
    std::string	elementTitle;

    elementTitle = splitEraseTrimChars(configData, "{");
    if (elementTitle != SERVER)
        throw std::runtime_error(E_CF_ELMNTDECL + elementTitle + '\n');
    return getInstruction(configData);
}

bool ConfigFileParser::_combineSharedNetAddress(const Configuration &currentConfig, size_t currentConfigIndex)
{
    for (size_t i = 0; i < currentConfigIndex; ++i)
    {
        if (_sharedNetAddress(currentConfig, _configs_[i]))
        {
            _configs_[i].addAltConfig(currentConfig);
            return true;
        }
    }
    return false;
}

bool ConfigFileParser::_sharedNetAddress(const Configuration &a, const Configuration &b)
{
    if (&a == &b)
        return false;
    if (a.getHost() == b.getHost() && a.getPort() == b.getPort())
        return true;
    return false;
}

//Public
ConfigFileParser::ConfigFileParser(const char *configFile)
{
    std::string	configData = _loadFile(configFile);

    if (RECV_CHUNK_SIZE < MAX_REQHEADSIZE)
        throw std::runtime_error(E_CF_SIZEMISMATCH);

    while (!configData.empty())
        _configs_.push_back(Configuration(_getConfigElement(configData)));

    if (_configs_.empty())
        throw std::runtime_error(E_CF_NOSERVER);

    if (_configs_.size() > 10)
        throw std::runtime_error(E_CF_MANYSERVER);

    for (size_t i = 0; i < _configs_.size(); ++i)
    {
        if (_combineSharedNetAddress(_configs_[i], i))
            _configs_.erase(_configs_.begin() + i--);
    }
    std::cout << I_CF_CONFIGIMPORT << std::endl;
}

std::vector<Configuration> ConfigFileParser::getConfigs() const
{
    return _configs_;
}
