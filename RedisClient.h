#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include<string>

class RedisClient {
private:
    int port;
    int sockfd;
    std::string host;
    
public:
    RedisClient(const std::string &host, int port);
    
    ~RedisClient();
    
    bool connectToServer();
    void disconnect();
    
    int getPort() const;
    int getSocketFd() const;
    std::string getHost() const;
    
    bool sendCommand(const std::string &command);
};

#endif