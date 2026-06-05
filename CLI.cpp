#include "CLI.h"

static std::string trim(const std::string &line) // helper to trim whitespace
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
        std::cout << redisClient.getHost() << ":" << redisClient.getPort() << "> ";
        std::cout.flush();

        std::string line;

        if (!std::getline(std::cin, line)) {
            break;
        }
        
        line = trim(line);

        if (line.empty()) {
            continue;
        }

        if (line == "quit" || line == "exit")
        {
            std::cout << "Goodbye from your client.\n";
            break;
        }
        else if (line == "help")
        {
            std::cout << "Displaying help\n";
            continue;
        }

        // split commands into tokens
        std::vector<std::string> args = CommandHandler::splitArgs(line);

        if (args.empty()) {
            continue;
        }

        std::string command = CommandHandler::buildRESPCommand(args);

        if (!redisClient.sendCommand(command))
        {
            std::cerr << "(Error) failed to send command.\n";
            break;
        }
        
        std::string res = ResponseParser::parseResponse(redisClient.getSocketFd());

        std::cout << res << std::endl;
    }

    redisClient.disconnect();
}