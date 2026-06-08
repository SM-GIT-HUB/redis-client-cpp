#include "RedisURL.h"
#include "RedisUrlParser.h"

#include <string>
#include <stdexcept>

RedisURL RedisUrlParser::parseRedisUrl(const std::string &url)
{
    RedisURL result;

    size_t schemePos = url.find("://");

    if (schemePos == std::string::npos) {
        throw std::runtime_error("Invalid Redis URL");
    }

    result.scheme = url.substr(0, schemePos);

    std::string remaining = url.substr(schemePos + 3);

    std::string hostPart;

    size_t atPos = remaining.find('@');

    if (atPos != std::string::npos)
    {
        std::string authPart = remaining.substr(0, atPos);
        hostPart = remaining.substr(atPos + 1);

        size_t colonPos = authPart.find(':');

        if (colonPos != std::string::npos)
        {
            result.username = authPart.substr(0, colonPos);
            result.password = authPart.substr(colonPos + 1);
        }
        else
            result.username = authPart;
    }
    else
        hostPart = remaining;

    size_t portPos = hostPart.rfind(':');

    if (portPos != std::string::npos)
    {
        result.host = hostPart.substr(0, portPos);
        result.port = std::stoi(hostPart.substr(portPos + 1));
    }
    else
        result.host = hostPart;

    return result;
}