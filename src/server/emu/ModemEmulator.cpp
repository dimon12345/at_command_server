#include <iostream>
#include <sstream>

#include "ModemEmulator.h"

namespace {
    const int AVAILABLE_NETWORK = 1;
    const int FORBIDDEN_NETWORK = 3;

    std::string SUPPORTED_MODES_AND_FORMATS = ",,(0,1,2,3,4),(0,1,2)";
    std::string HARDCODED_SIM_PIN_NUMBER = "4321";
}

ModemEmulator::ModemEmulator() {
    operators_.push_back({AVAILABLE_NETWORK, "MTS RUS", "MTS", "25001"});
    operators_.push_back({FORBIDDEN_NETWORK, "Megafon RUS", "Megafon", "25002"});
    operators_.push_back({FORBIDDEN_NETWORK, "Beeline", "Beeline", "25099"});

    mode_ = 0;
    format_ = 0;
    operator_index_ = 0;
    registered_ = true;
    sim_pin_locked_ = true;
}

bool ModemEmulator::isSimPinLocked() {
    return sim_pin_locked_;
}

bool ModemEmulator::unlockSmsPin(std::string_view pin) {
    if (pin == HARDCODED_SIM_PIN_NUMBER) {
        sim_pin_locked_ = false;
        return true;
    }

    return false;
}

bool ModemEmulator::isRegisteredNetwork() {
    return registered_;
}

void ModemEmulator::deregisterNetwork() {
    registered_ = false;
}

void ModemEmulator::setAutoOperatorSelection() {
    mode_ = 0;
    operator_index_ = 0;
    registered_ = true;
}

CmeError ModemEmulator::setOperator(int format, std::string_view oper) {
    if (oper.size() < 3 || oper[0] != '\"' || oper[oper.size()-1] != '\"') {
        return ErrorError();
    }

    format_ = format;
    mode_ = 1;
    oper = oper.substr(1, oper.size()-2);

    for(size_t index = 0; index < operators_.size(); ++index) {
        Operator &op = operators_[index];
        if ((format == 0 && op.long_name == oper) ||
                (format == 1 && op.short_name == oper) ||
                (format == 2 && op.plmn == oper)) {
            if (op.stat != 1) {
                registered_ = false;
                return NoNetworkCmeError();
            }
            operator_index_ = index;
            registered_ = true;
            return SuccessError();
        }
    }
    return NotFoundCmeError();
}

void ModemEmulator::setMode(int mode) {
    mode_ = mode;
}

int ModemEmulator::getMode() {
    return mode_;
}

void ModemEmulator::setFormat(int format) {
    format_ = format;
}

int ModemEmulator::getFormat() {
    return format_;
}

std::string ModemEmulator::getSelectionOfCommonOperator() {
    if (!registered_) {
        return std::to_string(mode_);
    }

    Operator &op = operators_[operator_index_];
    std::string oper;
    switch(format_) {
        case 0:
            oper = op.long_name;
            break;
        case 1:
            oper = op.short_name;
            break;
        case 2:
            oper = op.plmn;
            break;
    }

    std::stringstream result_stream;
    result_stream << mode_ << "," << format_ << ",\"" << oper << "\"";
    return result_stream.str();
}

std::string ModemEmulator::getAvailableOperators() {
    std::stringstream result_stream;
    int index = 0;
    for (const auto &oper: operators_) {
        if (result_stream.tellp() != 0) {
            result_stream << ",";
        }
        int stat = oper.stat;
        if (index++ == operator_index_ && registered_) {
            stat = 2;
        }
        result_stream << "(" << stat << ",\"" << oper.long_name << "\",\"" <<
                      oper.short_name << "\",\"" << oper.plmn << "\")";
    }
    result_stream << SUPPORTED_MODES_AND_FORMATS;
    return result_stream.str();
}
