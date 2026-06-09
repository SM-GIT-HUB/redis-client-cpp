#include "CLI.h"
#include "RedisUrlParser.h"

#include<string>
#include<vector>
#include<iostream>

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    std::cout << "This is your own Redis client::" << std::endl;
    
    int i = 1;
    int port = 6379;
    bool useTLS = false;
    std::string host = "127.0.0.1";

    std::string username = "";
    std::string password = "";

    std::vector<std::string> commandArgs;

    // parse command-line argv for -h and -p

    while (i < argc)
    {
        std::string arg = argv[i];
        
        if (arg == "-url")
        {
            RedisURL parsed = RedisUrlParser::parseRedisUrl(argv[++i]);

            host = parsed.host;
            port = parsed.port;
            username = parsed.username;
            password = parsed.password;
            useTLS = (parsed.scheme == "rediss");
        }
        else if (arg == "-h" && i + 1 < argc) {
            host = argv[++i];
        }
        else if (arg == "-p" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        }
        else if (arg == "-u" && i + 1 < argc) {
            username = argv[++i];
        }
        else if (arg == "-pass" && i + 1 < argc) {
            password = argv[++i];
        }
        else
        {
            // remaining arguments

            while (i < argc)
            {
                commandArgs.push_back(argv[i++]);
            }

            break;
        }
        
        ++i;
    }

    CLI cli(host, port, username, password, useTLS);
    cli.run(commandArgs); // execute oneshot command with commandArgs
    
    return 0;
}