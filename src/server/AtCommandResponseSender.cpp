#include <string>
#include <sstream>

#include "AtCommandResponseSender.h"

namespace {
    const std::string_view OK_RESPONSE = "OK";
    const std::string_view ERROR_RESPONSE = "ERROR";
}

void AtCommandResponseSender::sendOkResponse() {
    sendResponse(OK_RESPONSE);
}

void AtCommandResponseSender::sendErrorResponse() {
    sendResponse(ERROR_RESPONSE);
}

bool AtCommandResponseSender::sendCommonErrorResponse(const CmeError &error) {
    if (error.error_code == CmeErrorCode::SUCCESS) {
        sendOkResponse();
        return true;
    }

    if (error.error_code == CmeErrorCode::ERROR) {
        sendErrorResponse();
        return true;
    }

    return false;
}
