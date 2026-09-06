#pragma once

#include <string>
#include <string_view>

#include "platform/CmeError.h"

class Modem {
public:
    virtual ~Modem() = default;

    virtual bool isSimPinLocked() = 0;
    virtual bool unlockSmsPin(std::string_view pin) = 0;

    virtual bool isRegisteredNetwork() = 0;
    virtual void deregisterNetwork() = 0;

    virtual void setAutoOperatorSelection() = 0;
    virtual CmeError setOperator(int format, std::string_view oper) = 0;

    virtual void setMode(int mode) = 0;
    virtual int getMode() = 0;

    virtual void setFormat(int format) = 0;
    virtual int getFormat() = 0;

    virtual std::string getSelectionOfCommonOperator() = 0;
    virtual std::string getAvailableOperators() = 0;
};
