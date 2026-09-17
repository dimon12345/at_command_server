#include <fstream>

#include "AtCommandDictionary.h"
#include "Config.h"

namespace {
    constexpr std::string_view EMPTY_STRING_VEW = "";

    class DictionaryFormatError : public std::invalid_argument {
    public:
        DictionaryFormatError(const std::string &dictionary_filename,
                              int line_index, const std::string &line) :
            std::invalid_argument(
                    "Bad dictionary format [" + dictionary_filename + ":" + std::to_string(line_index) +
                    "]: '" + line + "'") {
        }
    };
}


AtCommandDictionary::AtCommandDictionary(const Config &config) {
    std::ifstream file(config.dictionary_filename);
    if (!file.is_open()) {
        std::string message = "Can't open dictionary file: " + config.dictionary_filename;
        throw std::runtime_error(message);
    }

    std::string line;
    int line_index = 0;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) {
            throw DictionaryFormatError(config.dictionary_filename, line_index, line);
        }

        std::string command = line.substr(0, delimiterPos);
        if (command.size() < 2 || command[0] != 'A' || command[1] != 'T') {
            throw DictionaryFormatError(config.dictionary_filename, line_index, line);
        }

        std::string response = line.substr(delimiterPos + 1);
        std::string at_command = command.substr(2);
        if (at_command.find_first_of(".*") == std::string::npos) {
            fixed_commands_.push_back({at_command, response});
        } else {
            pattern_commands_.push_back({at_command, response});
        }

        ++line_index;
    }
}

const std::vector<AtCommandDictionary::command_response_pair_t> &AtCommandDictionary::getFixedCommands() const {
    return fixed_commands_;
}

std::string_view AtCommandDictionary::parseCommand(std::string_view command) const
{
    for (const auto &pattern: pattern_commands_) {
        if (isMatch(command, pattern.first)) {
            return pattern.second;
        }
    }

    return EMPTY_STRING_VEW;
}

// Bottom-Up Dynamic Programming с оптимизацией памяти.
bool AtCommandDictionary::isMatch(std::string_view command, std::string_view pattern) const {
    size_t n = command.length();
    size_t m = pattern.length();

    std::vector<bool> dp(m + 1, false);
    std::vector<bool> prev_dp(m + 1, false);

    prev_dp[0] = true;

    for (size_t j = 0 ; j < m; ++j) {
        if (pattern[j] == '*') {
            prev_dp[j] = prev_dp[j];
        }
    }

    for (size_t i = 0; i < n; ++i) {
        std::fill(dp.begin(), dp.end(), false);

        for (int j = 0; j < m; ++j) {
            if (pattern[j] == '*') {
                dp[j + 1] = dp[j] || prev_dp[j + 1];
            } else if (pattern[j] == '.' || pattern[j] == command[i]) {
                dp[j + 1] = prev_dp[j];
            } else {
                dp[j + 1] = false;
            }
        }

        prev_dp = dp;
    }

    return prev_dp[m];
}
