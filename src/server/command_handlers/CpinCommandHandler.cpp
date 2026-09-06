#include "CpinCommandHandler.h"
#include "platform/Modem.h"
#include "server/AtCommandResponseSender.h"

namespace {
    std::string SIM_PIN_LOCKED_RESPONSE = "SIM PIN";
    std::string SIM_PIN_UNLOCKED_RESPONSE = "READY";
    std::string SIM_PIN_RESPONSE_PREFIX = "+CPIN: ";
}

void CpinCommandHandler::handleStatusCommand() {
    std::string response(SIM_PIN_RESPONSE_PREFIX);
    if (modem_.isSimPinLocked()) {
        response += SIM_PIN_LOCKED_RESPONSE;
    } else {
        response += SIM_PIN_UNLOCKED_RESPONSE;
    }
    sender_.sendResponse(response);
    sender_.sendOkResponse();
}

void CpinCommandHandler::handle(std::string_view command) {
    if (command.size() < 2 || command[0] != '\"' || command[command.size() - 1] != '\"') {
        sender_.sendErrorResponse();
    }
    std::string_view pin = command.substr(1, command.size() - 2);
    if (modem_.unlockSmsPin(pin)) {
        sender_.sendOkResponse();
    } else {
        sender_.sendCmeErrorResponse(IncorrectPasswordCmeError());
    }
}
