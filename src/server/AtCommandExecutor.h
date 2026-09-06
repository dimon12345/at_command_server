#pragma once

#include <functional>
#include <string_view>
#include <vector>
#include <string>
#include <unordered_map>

#include "dictionary/AtCommandDictionary.h"
#include "command_handlers/CopsCommandHandler.h"
#include "command_handlers/CpinCommandHandler.h"
#include "emu/ModemEmulator.h"

class AtCommandExecutor {
public:
    AtCommandExecutor(const class Config &config, class AtCommandServerState &state, class AtCommandResponseSender &sender);

    // execute() return true if command was handled
    bool execute(std::string_view command);

private:
    const class Config &config_;
    class AtCommandServerState &state_;
    class AtCommandResponseSender &sender_;

    AtCommandDictionary dictionary_;
    ModemEmulator modem_;
    CopsCommandHandler cops_command_handler_;
    CpinCommandHandler cpin_command_handler_;

    typedef std::unordered_map<std::string, std::function<void(std::string_view)>> commands_map_t;
    commands_map_t commands_;
};
