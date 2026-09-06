#pragma once

#include "server/AtCommandHandler.h"

class CopsCommandHandler : public AtCommandHandler {
public:
    CopsCommandHandler(class AtCommandResponseSender &sender, class Modem &modem)
        : AtCommandHandler(sender), modem_(modem) {}
    void handleStatusCommand();

    void handle(std::string_view command) override;
private:
    class Modem &modem_;

    void handleCopsCommand(std::string_view mode, std::string_view format, std::string_view oper);
};
