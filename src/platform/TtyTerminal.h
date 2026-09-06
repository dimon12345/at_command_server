#pragma once

#include <span>
#include <string>
#include <string_view>

#include "UniqueFileDescriptor.h"

class TtyTerminal {
public:
    TtyTerminal();
    size_t readData(std::span<char> buffer) const;
    size_t writeData(std::string_view data) const;
    int bytesAvailable() const;
    void sendAtCommandResponse(std::string_view response);
private:
    UniqueFileDescriptor tty_master_fd_;
    std::string tty_slave_name_;
};
