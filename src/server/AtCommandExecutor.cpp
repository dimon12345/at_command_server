#include "AtCommandExecutor.h"
#include "AtCommandServerState.h"
#include "AtCommandResponseSender.h"
#include "Config.h"

namespace {
    std::string ECHO_DISABLE_COMMAND = "E0";
    std::string ECHO_ENABLE_COMMAND = "E1";
    std::string COPS_STATUS_COMMAND = "+COPS?";
    std::string COPS_COMMAND = "+COPS=";
    std::string CPIN_STATUS_COMMAND = "+CPIN?";
    std::string CPIN_COMMAND = "+CPIN=";
}

AtCommandExecutor::AtCommandExecutor(const Config &config, AtCommandServerState &state, AtCommandResponseSender &sender)
        : config_(config), state_(state), sender_(sender),
          dictionary_(config), cops_command_handler_(sender, modem_),
          cpin_command_handler_(sender, modem_) {

    commands_[ECHO_DISABLE_COMMAND] = [this](std::string_view command) {
        state_.echo_enabled = false;
        sender_.sendOkResponse();
    };
    commands_[ECHO_ENABLE_COMMAND] = [this](std::string_view command) {
        state_.echo_enabled = true;
        sender_.sendOkResponse();
    };
    commands_[COPS_STATUS_COMMAND] = [this](std::string_view command) {
        cops_command_handler_.handleStatusCommand();
    };
    commands_[COPS_COMMAND] = [this](std::string_view command) {
        cops_command_handler_.handle(command.substr(6));
    };
    commands_[CPIN_STATUS_COMMAND] = [this](std::string_view command) {
        cpin_command_handler_.handleStatusCommand();
    };
    commands_[CPIN_COMMAND] = [this](std::string_view command){
        cpin_command_handler_.handle(command.substr(6));
    };

    auto &fixed_commands = dictionary_.getFixedCommands();
    for (size_t i = 0; i < fixed_commands.size(); ++i) {
        auto &fixed_command = fixed_commands[i];
        commands_[fixed_command.first] = [this, &fixed_command](std::string_view command){
            sender_.sendResponse(fixed_command.second);
            sender_.sendOkResponse();
        };
    }
}

bool AtCommandExecutor::execute(std::string_view command) {
    if (command.empty() || command == "\r") {
        sender_.sendOkResponse();
        return true;
    }

    std::string stripped_command(command);
    size_t iter = command.find('=');
    if (iter != std::string::npos) {
        stripped_command.erase(iter + 1);
    }

    auto command_item = commands_.find(stripped_command);
    if (command_item != commands_.end()) {
        command_item->second(command);
        return true;
    }

    std::string_view response = dictionary_.parseCommand(command);
    if (!response.empty()) {
        sender_.sendResponse(response);
        sender_.sendOkResponse();
        return true;
    }
    return false;
}
