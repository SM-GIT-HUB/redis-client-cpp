#ifndef CLI_H
#define CLI_H

#include<string>
#include<vector>

#include "RedisClient.h"

class CLI {
private:
    RedisClient redisClient;
    
public:
    CLI(const std::string &host, int port);

    void run(const std::vector<std::string>& commandArgs);
    void executeCommand(const std::vector<std::string>& commandArgs);
};

#endif