#include "ResponseParser.h"

#include<string>
#include<sys/socket.h>
#include<iostream>

static bool readChar(int sockfd, char &c) // read a single character from the socket
{
    ssize_t r = recv(sockfd, &c, 1, 0);
    return (r == 1);
}

static std::string readLine(int sockfd) // read a line of text from the socket until it encounters a carriage return
{
    char c;
    std::string line;
    
    while (readChar(sockfd, c))
    {
        if (c == '\r')
        {
            // expect '\n' next; read and break;

            readChar(sockfd, c);
            break;
        }

        line.push_back(c);
    }

    std::cout << line << "  line  " << std::endl;
    return line;
}

std::string ResponseParser::parseResponse(int sockfd)
{
    char prefix;

    if (!readChar(sockfd, prefix)) {
        return ("(Error) no response or connection closed.");
    }

    switch (prefix)
    {
        case '+' :
            return parseSimpleString(sockfd);
        case '-' :
            return parseSimpleError(sockfd);
        case ':' :
            return parseInteger(sockfd);
        case '$' :
            return parseBulkString(sockfd);
        case '*' :
            return parseArrays(sockfd);
        default:
            return "(Error) unknown reply type.";
    }
}

std::string ResponseParser::parseSimpleString(int sockfd)
{
    return readLine(sockfd);
}

std::string ResponseParser::parseSimpleError(int sockfd)
{
    return readLine(sockfd);
}

std::string ResponseParser::parseInteger(int sockfd)
{   return readLine(sockfd);
}

std::string ResponseParser::parseBulkString(int sockfd)
{   return readLine(sockfd);
}

std::string ResponseParser::parseArrays(int sockfd)
{   return readLine(sockfd);
}
