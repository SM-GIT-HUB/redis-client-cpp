#include<iostream>
#include<string>
#include "CLI.h"

int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;
    std::cout << "This is your own Redis client::" << std::endl;
    
    std::string host = "127.0.0.1";
    int port = 6379;
    int i = 1;

    // parse command-line argv for -h and -p

    while (i < argc)
    {
        std::string arg = argv[i];

        if (arg == "-h" && i + 1 < argc) {
            host = argv[++i];
        }
        else if (arg == "-p" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        }
        else
            break;
        
        ++i;
    }

    CLI cli(host, port);
    cli.run();
    
}