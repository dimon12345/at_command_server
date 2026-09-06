#pragma once

#include <string_view>

#include "platform/CmeError.h"


class AtCommandResponseSender {
public:
    virtual ~AtCommandResponseSender() = default;
    virtual void sendResponse(std::string_view response) = 0;
    virtual void sendCmeErrorResponse(const CmeError &error) = 0;

    void sendOkResponse();
    void sendErrorResponse();

protected:
    static constexpr std::string_view CME_ERROR_RESPONSE_PREFIX = "+CME ERROR: ";

    bool sendCommonErrorResponse(const CmeError &error);
};
