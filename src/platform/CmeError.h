#pragma once

enum CmeErrorCode {
    SUCCESS = -2,
    ERROR = -1,
    SIM_PIN_REQUIRED = 11,
    INCORRECT_PASSWORD = 16,
    NOT_FOUND=22,
    NO_NETWORK_SERVICE_ERROR=30,
};


struct CmeError {
    int error_code;
    std::string description;
};


class SuccessError : public CmeError {
public:
    SuccessError() : CmeError({CmeErrorCode::SUCCESS, ""}) {}
};

class ErrorError : public CmeError {
public:
    ErrorError() : CmeError({CmeErrorCode::ERROR, ""}) {}
};

class NotFoundCmeError : public CmeError {
public:
    NotFoundCmeError() : CmeError({CmeErrorCode::NOT_FOUND, "not found"}) {}
};


class NoNetworkCmeError : public CmeError {
public:
    NoNetworkCmeError() : CmeError({CmeErrorCode::NO_NETWORK_SERVICE_ERROR, "no network service"}) {}
};


class SimPinRequiredCmeError : public CmeError {
public:
    SimPinRequiredCmeError() : CmeError({CmeErrorCode::SIM_PIN_REQUIRED, "SIM PIN required"}) {}
};


class IncorrectPasswordCmeError : public CmeError {
public:
    IncorrectPasswordCmeError() : CmeError({CmeErrorCode::INCORRECT_PASSWORD, "incorrect password"}) {}
};
