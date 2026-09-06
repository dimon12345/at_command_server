#pragma once

#include <string>
#include <string_view>

#include "server/AtCommandResponseSender.h"

class TestAtCommandResponseSender : public AtCommandResponseSender {
public:
    TestAtCommandResponseSender(const class Config &config);
    const std::string &getSession() const;
    void addCommand(std::string_view command);
    void sendResponse(std::string_view response) override;
    void sendCmeErrorResponse(const CmeError &error) override;

private:
    std::string session_;
    const class Config &config_;
};
