#pragma once

#include <vector>
#include <string>
#include <string_view>

#include "platform/Modem.h"


class ModemEmulator : public Modem {
public:
    ModemEmulator();

    bool isSimPinLocked() override;
    bool unlockSmsPin(std::string_view pin) override;

    bool isRegisteredNetwork() override;
    void deregisterNetwork() override;

    void setAutoOperatorSelection() override;
    CmeError setOperator(int format, std::string_view oper) override;

    void setMode(int mode) override;
    int getMode() override;

    void setFormat(int format) override;
    int getFormat() override;

    std::string getSelectionOfCommonOperator() override;
    std::string getAvailableOperators() override;

private:
    bool sim_pin_locked_;
    bool registered_;
    int mode_;
    int format_;
    int operator_index_;

    struct Operator {
        int stat;
        std::string long_name;
        std::string short_name;
        std::string plmn;
    };

    std::vector<Operator> operators_;
};
