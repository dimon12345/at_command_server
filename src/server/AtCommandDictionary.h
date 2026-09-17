#pragma once

#include <stdexcept>
#include <string_view>
#include <vector>

class AtCommandDictionary {
public:
    AtCommandDictionary(const class Config &config);

    typedef std::pair<std::string, std::string> command_response_pair_t;
    const std::vector<command_response_pair_t> &getFixedCommands() const;

    std::string_view parseCommand(std::string_view command) const;

private:
    std::vector<command_response_pair_t> fixed_commands_;
    std::vector<command_response_pair_t> pattern_commands_;
};
