#include "ResponseParser.h"

#include<string>
#include<sstream>
#include<iostream>
#include<sys/socket.h>

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
    return "(Error) " + readLine(sockfd);
}

std::string ResponseParser::parseInteger(int sockfd)
{
    return "(ineteger) " + readLine(sockfd);
}

std::string ResponseParser::parseBulkString(int sockfd)
{
    int len = std::stoi(readLine(sockfd));

    if (len == -1) {
        return "(nil)";
    }

    std::string bulk(len, '\0');

    ssize_t totalRead = 0;

    while (totalRead < len)
    {
        ssize_t r = recv(sockfd, &bulk[totalRead], len - totalRead, 0); // read bulk data from socket

        if (r <= 0) {
            return "(Error) incomplete bulk data.";
        }

        totalRead += r;
    }

    // consume trailing CRLF
    char dummy;
    readChar(sockfd, dummy);
    readChar(sockfd, dummy);

    return bulk;
}

std::string ResponseParser::parseArrays(int sockfd)
{
    int count = std::stoi(readLine(sockfd));
    
    if (count == -1) {
        return "(nil)";
    }

    std::ostringstream oss;

    for (int i = 0; i < count; i++)
    {
        oss << parseResponse(sockfd);

        if (i != count - 1) {
            oss << "\n";
        }
    }

    return oss.str();
}
