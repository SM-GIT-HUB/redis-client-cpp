#include "CLI.h"
#include "CommandHandler.h"
#include "ResponseParser.h"

#include<iostream>
#include<readline/history.h>
#include<readline/readline.h>

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

CLI::CLI(const std::string &host, int port, const std::string &username, const std::string &password, bool useTLS) : redisClient(host, port, useTLS), username(username), password(password) {}

void CLI::run(const std::vector<std::string>& commandArgs)
{
    if (!redisClient.connectToServer()) {
        return;
    }

    if (!password.empty())
    {
        std::vector<std::string> authArgs;

        if (username.empty())
            authArgs = {"AUTH", password};
        else
            authArgs = {"AUTH", username, password};

        executeCommand(authArgs);
    }

    if (!commandArgs.empty()) // execute oneshot command
    {
        executeCommand(commandArgs);
        redisClient.disconnect();
        return;
    }

    std::cout << "Connected to Redis server" << std::endl;

    while (true)
    {
        std::string prompt = "\n" + redisClient.getHost() + ":" + std::to_string(redisClient.getPort()) + "> ";

        char* input = readline(prompt.c_str());

        if (!input) {
            break;
        }

        std::string line(input);

        if (!line.empty()) {
            add_history(input);
        }

        free(input);
        
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
            std::cout
                << "\nRedis CLI Client Help\n"
                << "=====================\n\n"

                << "Built-in Commands:\n"
                << "  help              Show this help message\n"
                << "  quit              Exit the client\n"
                << "  exit              Exit the client\n\n"

                << "Examples:\n"
                << "  PING\n"
                << "  SET name Soumik\n"
                << "  GET name\n"
                << "  DEL name\n"
                << "  KEYS *\n"
                << "  HSET user name Soumik\n"
                << "  HGETALL user\n\n"

                << "Quoted Strings:\n"
                << "  SET name \"Soumik Majumder\"\n\n"

                << "Connection Options:\n"
                << "  -h <host>\n"
                << "  -p <port>\n"
                << "  -u <username>\n"
                << "  -pass <password>\n"
                << "  -url <redis-url>\n\n"

                << "URL Examples:\n"
                << "  redis://localhost:6379\n"
                << "  redis://:password@localhost:6379\n"
                << "  redis://default:password@localhost:6379\n"
                << "  rediss://default:password@host:6379\n"

                << std::endl;

            continue;
        }

        // split commands into tokens
        std::vector<std::string> args = CommandHandler::splitArgs(line);

        if (args.empty()) {
            continue;
        }

        executeCommand(args);
    }

    redisClient.disconnect();
}

void CLI::executeCommand(const std::vector<std::string>& args)
{
    std::string command = CommandHandler::buildRESPCommand(args);

    if (!redisClient.sendCommand(command))
    {
        std::cerr << "(Error) failed to send command.\n";
        return;
    }

    std::string res = ResponseParser::parseResponse(redisClient);

    if (args[0] == "AUTH")
    {
        if (res.find("(Error)") != std::string::npos)
        {
            std::cerr << res << std::endl;
            return;
        }
    }
    else
        std::cout << res << std::endl;
}