#ifndef CLI_H
#define CLI_H

#include "RedisClient.h"

#include<string>
#include<vector>

class CLI {
private:
    RedisClient redisClient;
    std::string username;
    std::string password;
    
public:
    CLI(const std::string &host, int port, const std::string &username, const std::string &password, bool useTLS);

    void run(const std::vector<std::string>& commandArgs);
    void executeCommand(const std::vector<std::string>& commandArgs);
};

#endif