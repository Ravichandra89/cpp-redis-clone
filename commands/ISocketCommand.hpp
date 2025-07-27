// ISocketCommand.hpp
#ifndef ISOCKET_COMMAND_HPP
#define ISOCKET_COMMAND_HPP

#include "ICommand.hpp"

class ISocketCommand : public ICommand
{
public:
    // Override standard execute() to disable default execution
    std::string execute(HashTable &, const std::vector<std::string> &) override
    {
        return "-ERR socket command not usable in standard command context\r\n";
    }

    virtual void execute(int clientSock, const std::vector<std::string> &args) = 0;

    virtual ~ISocketCommand() = default;
};

#endif // ISOCKET_COMMAND_HPP
