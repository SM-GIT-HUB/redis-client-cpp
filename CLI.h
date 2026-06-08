#ifndef CLI_H
#define CLI_H

#include<string>
#include<vector>

#include "RedisClient.h"

class CLI {
private:
    RedisClient redisClient;
    std::string username;
    std::string password;
    
public:
    CLI(const std::string &host, int port, const std::string &username, const std::string &password);

    void run(const std::vector<std::string>& commandArgs);
    void executeCommand(const std::vector<std::string>& commandArgs);
};

#endif