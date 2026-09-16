#pragma once

#include <string_view>

class AtCommandHandler {
public:
    AtCommandHandler(class AtCommandResponseSender &sender) : sender_(sender) {}
    virtual ~AtCommandHandler() = default;

protected:
    class AtCommandResponseSender &sender_;
};
