#pragma once

#include "server/AtCommandHandler.h"

class CpinCommandHandler : public AtCommandHandler {
public:
    CpinCommandHandler(class AtCommandResponseSender &sender, class Modem &modem)
        : AtCommandHandler(sender), modem_(modem) {}
    void handleStatusCommand();
    void handle(std::string_view command);
private:
    class Modem &modem_;
};
