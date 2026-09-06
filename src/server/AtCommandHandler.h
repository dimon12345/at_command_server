#pragma once

#include <string_view>

class AtCommandHandler {
public:
    AtCommandHandler(class AtCommandResponseSender &sender) : sender_(sender) {}
    virtual ~AtCommandHandler() = default;
    virtual void handle(std::string_view command) = 0;

protected:
    class AtCommandResponseSender &sender_;
};
