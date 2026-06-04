#include "RedisClient.h"

RedisClient::RedisClient(const std::string &host, int port) : host(host), port(port), sockfd(-1) {}

RedisClient::~RedisClient()
{
    disconnect();
}

bool RedisClient::connectToServer()
{
    struct addrinfo hints;
    struct addrinfo *res = nullptr;
    
    std::memset(&hints, 0, sizeof(hints)); // clear the hints

    hints.ai_family = AF_UNSPEC; // unspecified, can be: Ipv4 pr Ipv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    std::string portStr = std::to_string(port); // convert port to string
    int err = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &res); // resolve address and get error message: 0 means no error

    if (err != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(err) << "\n";
        return false;
    }

    for (auto p = res; p != nullptr; p = p->ai_next) // res is a pointer to a linked list
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); // create socket

        if (sockfd == -1) {
            continue; // failed, don't use
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) {
            break; // if connected, no need to loop any more
        }
        
        disconnect();
    }

    freeaddrinfo(res); // free the address information

    if (sockfd == -1)
    {
        std::cerr << "Could not connect to " << host << " " << port << "\n";
        return false; // if no connection was made
    }

    return true; // connection successful
}

void RedisClient::disconnect()
{
    if (sockfd != -1)
    {
        close(sockfd); // close socket if connection failed
        sockfd = -1; // reset socket file description
    }
}

int RedisClient::getPort() const {
    return port;
}

int RedisClient::getSocketFd() const {
    return sockfd;
}

std::string RedisClient::getHost() const {
    return host;
}

bool RedisClient::sendCommand(const std::string &command) //sending actual data(bytes) to redis
{
    if (sockfd == -1) {
        return false;
    }

    size_t totalSent = 0;

    while (totalSent < command.size())
    {
        ssize_t sent = send(sockfd, command.c_str() + totalSent, command.size() - totalSent, 0);

        if (sent <= 0) {
            return false;
        }

        totalSent += sent;
    }

    return true;
}