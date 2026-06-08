#ifndef REDIS_URL
#define REDIS_URL

#include<string>

struct RedisURL {
    std::string scheme;
    std::string username;
    std::string password;
    std::string host;
    int port = 6379;
};

#endif