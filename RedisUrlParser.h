#ifndef REDIS_URL_PARSER
#define REDIS_URL_PARSER

#include "RedisURL.h"

#include<string>

class RedisUrlParser {
public:
    static RedisURL parseRedisUrl(const std::string &url);
};

#endif