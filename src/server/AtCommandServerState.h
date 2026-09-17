#pragma once

struct AtCommandServerState {
    // TS 127 007: 4.3 Recommended default settings for E is 1
    bool echo_enabled = true;

    // verbose CMEE value
    int cmee_value = 2;
};
