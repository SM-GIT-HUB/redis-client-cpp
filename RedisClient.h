#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include<string>
#include<openssl/ssl.h>
#include<openssl/err.h>

class RedisClient {
private:
    int port;
    int sockfd;
    std::string host;

    bool useTLS;

    SSL_CTX* ctx;
    SSL* ssl;
    
public:
    RedisClient(const std::string &host, int port, bool useTLS);
    
    ~RedisClient();
    
    bool connectToServer();
    void disconnect();
    
    int getPort() const;
    int getSocketFd() const;
    std::string getHost() const;
    
    int readBytes(char* buffer, int len);
    bool sendCommand(const std::string &command);
};

#endif