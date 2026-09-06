#pragma once

struct AtCommandServerState {
    // V.250 6.2.4 Recommended default settings for E is 1
    bool echo_enabled = true;

    // verbose CMEE value
    int cmee_value = 2;
};
