#include <sstream>
#include <iostream>

#include "TestAtCommandResponseSender.h"
#include "Config.h"

TestAtCommandResponseSender::TestAtCommandResponseSender(const Config &config) : config_(config) {
}

const std::string &TestAtCommandResponseSender::getSession() const {
    return session_;
}

void TestAtCommandResponseSender::addCommand(std::string_view command) {
    session_ += command;
    if (config_.verbose) {
        std::cout << command;
    }
}

void TestAtCommandResponseSender::sendResponse(std::string_view response) {
    std::stringstream response_data;
    response_data << "\r\n" << response << "\r\n";
    if (config_.verbose) {
        std::cout << response_data.str();
    }
    session_ += response_data.str();
}

void TestAtCommandResponseSender::sendCmeErrorResponse(const CmeError &error) {
    if (sendCommonErrorResponse(error)) {
        return;
    }

    std::stringstream cme_error;
    cme_error << CME_ERROR_RESPONSE_PREFIX << error.description;
    sendResponse(cme_error.str());
}
