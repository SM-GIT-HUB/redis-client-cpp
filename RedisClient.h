#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include<string>
#include<iostream>

#include<netdb.h>
#include<unistd.h>
#include<sys/socket.h>
#include<cstring>

class RedisClient {
private:
    std::string host;
    int port;
    int sockfd;
    
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