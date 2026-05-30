#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLET_H

#include<vector>
#include<string>

class CommandHandler {
public:
    static std::vector<std::string> splitArgs(const std::string &input);
};

#endif