#ifndef REDIS_URL
#define REDIS_URL

#include<string>

struct RedisURL {
    int port = 6379;
    std::string host;
    std::string scheme;
    std::string username;
    std::string password;
};

#endif