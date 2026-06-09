#include "ResponseParser.h"

#include<string>
#include<sstream>
#include<iostream>
#include<sys/socket.h>

static bool readChar(RedisClient &client, char &c)
{
    ssize_t r = client.readBytes(&c, 1);
    return (r == 1);
}

static std::string readLine(RedisClient &client) // read a line of text from the socket until it encounters a carriage return
{
    char c;
    std::string line;
    
    while (readChar(client, c))
    {
        if (c == '\r')
        {
            // expect '\n' next; read and break;

            readChar(client, c);
            break;
        }

        line.push_back(c);
    }
    
    return line;
}

std::string ResponseParser::parseResponse(RedisClient &client)
{
    char prefix;

    if (!readChar(client, prefix)) {
        return ("(Error) no response or connection closed.");
    }

    switch (prefix)
    {
        case '+' :
            return parseSimpleString(client);
        case '-' :
            return parseSimpleError(client);
        case ':' :
            return parseInteger(client);
        case '$' :
            return parseBulkString(client);
        case '*' :
            return parseArrays(client);
        default:
            return "(Error) unknown reply type.";
    }
}

std::string ResponseParser::parseSimpleString(RedisClient &client)
{
    return readLine(client);
}

std::string ResponseParser::parseSimpleError(RedisClient &client)
{
    return "(Error) " + readLine(client);
}

std::string ResponseParser::parseInteger(RedisClient &client)
{
    return "(integer) " + readLine(client);
}

std::string ResponseParser::parseBulkString(RedisClient &client)
{
    int len = std::stoi(readLine(client));

    if (len == -1) {
        return "(nil)";
    }

    std::string bulk(len, '\0');

    ssize_t totalRead = 0;

    while (totalRead < len)
    {
        ssize_t r = client.readBytes(&bulk[totalRead], len - totalRead); // read bulk data from socket

        if (r <= 0) {
            return "(Error) incomplete bulk data.";
        }

        totalRead += r;
    }

    // consume trailing CRLF
    char dummy;
    readChar(client, dummy);
    readChar(client, dummy);

    return bulk;
}

std::string ResponseParser::parseArrays(RedisClient &client)
{
    int count = std::stoi(readLine(client));
    
    if (count == -1) {
        return "(nil)";
    }

    std::ostringstream oss;

    for (int i = 0; i < count; i++)
    {
        oss << parseResponse(client);

        if (i != count - 1) {
            oss << "\n";
        }
    }

    return oss.str();
}
