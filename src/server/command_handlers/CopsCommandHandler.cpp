#include <stdexcept>

#include "CopsCommandHandler.h"
#include "platform/Modem.h"
#include "server/AtCommandResponseSender.h"

namespace {
    std::string COPS_RESPONSE_PREFIX = "+COPS: ";

    std::tuple<std::string_view, std::string_view> cutCommaParam(std::string_view command) {
        size_t pos = command.find(',');
        if (pos == std::string_view::npos) {
            return {command, ""};
        }
        return {command.substr(0, pos), command.substr(pos + 1)};
    }
}

void CopsCommandHandler::handleStatusCommand() {
    if (modem_.isSimPinLocked()) {
        sender_.sendCmeErrorResponse(SimPinRequiredCmeError());
    } else {
        std::string response(COPS_RESPONSE_PREFIX);
        response += modem_.getSelectionOfCommonOperator();
        sender_.sendResponse(response);
        sender_.sendOkResponse();
    }
}

void CopsCommandHandler::handle(std::string_view command) {
    if (modem_.isSimPinLocked()) {
        sender_.sendCmeErrorResponse(SimPinRequiredCmeError());
        return;
    }

    if (command.empty()) {
        sender_.sendErrorResponse();
        return;
    }

    if (command.size() == 1 && command[0] == '?') {
        std::string response = COPS_RESPONSE_PREFIX + modem_.getAvailableOperators();
        sender_.sendResponse(response);
        sender_.sendOkResponse();
        return;
    }

    std::string_view mode = "";
    std::tie(mode, command) = ::cutCommaParam(command);

    std::string_view format = "";
    if (!command.empty()) {
        std::tie(format, command) = ::cutCommaParam(command);
    }

    std::string_view oper = "";
    if (!command.empty()) {
        oper = command;
    }

    handleCopsCommand(mode, format, oper);
}

void CopsCommandHandler::handleCopsCommand(std::string_view mode, std::string_view format, std::string_view oper) {
    if (mode.size() != 1) {
        sender_.sendErrorResponse();
        return;
    }

    int format_index = modem_.getFormat();
    if (!format.empty()) {
        try {
            format_index = std::stoi(std::string(format));
            if (format_index < 0 || format_index > 2) {
                sender_.sendErrorResponse();
                return;
            }
        } catch (const std::logic_error &e) {
            sender_.sendErrorResponse();
            return;
        }
    }

    switch(mode[0]) {
        case '0':
            modem_.setAutoOperatorSelection();
            sender_.sendOkResponse();
            break;
        case '1':
            modem_.setFormat(format_index);
            modem_.setMode(1);
            {
                CmeError error = modem_.setOperator(format_index, oper);
                sender_.sendCmeErrorResponse(error);
            }
            break;
        case '2':
            modem_.deregisterNetwork();
            modem_.setMode(2);
            sender_.sendOkResponse();
            break;
        case '3':
            modem_.setFormat(format_index);
            sender_.sendOkResponse();
            break;
        case '4':
            {
                CmeError error = modem_.setOperator(format_index, oper);
                if (error.error_code == CmeErrorCode::SUCCESS){
                    sender_.sendOkResponse();
                } else if (error.error_code ==
                        CmeErrorCode::NO_NETWORK_SERVICE_ERROR ||
                        error.error_code == CmeErrorCode::NOT_FOUND) {
                    modem_.setAutoOperatorSelection();
                    sender_.sendOkResponse();
                } else {
                    sender_.sendErrorResponse();
                }
            }
            break;
        default:
            sender_.sendErrorResponse();
            break;
    }
}
