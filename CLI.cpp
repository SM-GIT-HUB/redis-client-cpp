#include "CLI.h"

// helper to trim whitespace

static std::string trim(const std::string &line)
{
    // whitespace characters are: ' ', \t, \n, \r, \f, \v

    int start = 0, end = int(line.size()) - 1;

    while (start <= end && std::isspace(line[start]))
    {
        start++;
    }

    while (start <= end && std::isspace(line[end]))
    {
        end--;
    }

    return (start > end)? "" : line.substr(start, end - start + 1);
}

CLI::CLI(const std::string &host, int port) : redisClient(host, port) {}

void CLI::run()
{
    if (!redisClient.connectToServer()) {
        return;
    }

    std::cout << "Connected to Redis server" << std::endl;

    while (true)
    {
        std::cout << redisClient.getHost() << ":" << redisClient.getPort() << ">";
        std::cout.flush();

        std::string line;

        if (!std::getline(std::cin, line)) {
            break;
        }
        
        line = trim(line);

        if (line.empty()) {
            continue;
        }

        if (line == "quit")
        {
            std::cout << "Goodbye from your client.\n";
            break;
        }

        std::cout << "Your input is: " << line << std::endl;
    }
}