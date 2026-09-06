#pragma once

#include <string_view>

#include "AtCommandResponseSender.h"
#include "AtCommandExecutor.h"
#include "AtCommandServerState.h"
#include "platform/TtyTerminal.h"

class AtCommandServer : public AtCommandResponseSender {
public:
    AtCommandServer(const class Config &config);
    int run();

    void sendResponse(std::string_view response) override;
    void sendCmeErrorResponse(const CmeError &error) override;
private:
    const class Config &config_;
    AtCommandServerState state_;
    std::string rx_buffer_;

    TtyTerminal terminal_;
    AtCommandExecutor executor_;

    void processRawData(std::string_view raw_data);
    void executeCommand(const std::string_view command);
};
