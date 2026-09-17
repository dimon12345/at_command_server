#include <iostream>
#include <sstream>

#include "Config.h"
#include "AtCommandServer.h"
#include "AtCommandExecutor.h"

namespace {
    const size_t AT_COMMAND_BUFFER_SIZE = 1024;
}

AtCommandServer::AtCommandServer(const class Config &config)
        : executor_(config, state_, *this) {
}

int AtCommandServer::run() {
    std::array<char, AT_COMMAND_BUFFER_SIZE> buffer{};
    while (true) {
        size_t bytes_read = terminal_.readData(buffer);
        if (bytes_read == 0) {
            std::cout << "Connection closed (EOF).\n";
            break;
        }

        // Buffer overflow
        if (bytes_read == buffer.size()) {
            while(terminal_.bytesAvailable()) {
                terminal_.readData(buffer);
            }
            sendErrorResponse();
            continue;
        }

        processRawData({buffer.data(), bytes_read});
    }
    return 0;
}

void AtCommandServer::processRawData(std::string_view raw_data) {
    for (char ch: raw_data) {
        // Backspace
        if (ch == '\b' || ch == 127) {
            if (!rx_buffer_.empty()) {
                rx_buffer_.pop_back();
                if (state_.echo_enabled) {
                    terminal_.writeData("\b \b");
                }
            }
            continue;
        }

        if (state_.echo_enabled && ch != '\r' && ch != '\n') {
            terminal_.writeData(std::string_view(&ch, 1));
        }

        if (ch == '\r') {
            if (state_.echo_enabled) {
                terminal_.writeData(std::string_view(&ch, 1));
            }
            executeCommand(rx_buffer_);
            rx_buffer_.clear();
        } else if(ch != '\n') {
            rx_buffer_.push_back(ch);
        }
    }
}

void AtCommandServer::executeCommand(std::string_view command) {
    if (!command.empty() && command.front() == '\n') {
        command.remove_prefix(1);
    }

    if (command.empty()) {
        return;
    }

    if (command.size() < 2 ||
            (std::toupper(command[0]) != 'A' || std::toupper(command[1])
            != 'T')) {
        sendErrorResponse();
        return;
    }

    std::string_view at_command = command.substr(2);
    if (!executor_.execute(at_command)) {
        sendErrorResponse();
    }
}

void AtCommandServer::sendResponse(std::string_view response) {
    // TS 127 007: 4.3 Recommended default settings for V is 1
    std::stringstream response_data;
    response_data << "\r\n" << response << "\r\n";
    terminal_.writeData(response_data.str());
}

void AtCommandServer::sendCmeErrorResponse(const CmeError &error) {
    if (sendCommonErrorResponse(error)) {
        return;
    }

    if (state_.cmee_value == 0) {
        sendErrorResponse();
        return;
    }

    std::stringstream cme_error;
    cme_error << CME_ERROR_RESPONSE_PREFIX;
    if (state_.cmee_value == 2) {
        cme_error << error.description;
    } else {
        cme_error << error.error_code;
    }
    sendResponse(cme_error.str());
}
