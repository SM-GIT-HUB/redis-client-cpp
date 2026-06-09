#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include "RedisClient.h"

#include<string>

class ResponseParser {
private:
    // parsing from Redis Serialization Protocol 2: according to the prefix or first character response
    static std::string parseArrays(RedisClient &client);
    static std::string parseInteger(RedisClient &client);
    static std::string parseBulkString(RedisClient &client);
    static std::string parseSimpleError(RedisClient &client);
    static std::string parseSimpleString(RedisClient &client);
public:
    static std::string parseResponse(RedisClient &client); // read from the given socket and return parsed response
};

#endif