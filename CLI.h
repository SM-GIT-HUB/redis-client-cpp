#ifndef CLI_H
#define CLI_H

#include<string>

#include "RedisClient.h"
#include "CommandHandler.h"
#include "ResponseParser.h"

class CLI {
private:
    RedisClient redisClient;
    
public:
    CLI(const std::string &host, int port);
    void run();
};

#endif