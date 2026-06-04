#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include<string>

class ResponseParser {
private:
    // parsing from Redis Serialization Protocol 2: according to the prefix or first character response
    static std::string parseSimpleString(int sockfd);
    static std::string parseSimpleError(int sockfd);
    static std::string parseInteger(int sockfd);
    static std::string parseBulkString(int sockfd);
    static std::string parseArrays(int sockfd);
public:
    static std::string parseResponse(int sockfd); // read from the given socket and return parsed response
};

#endif