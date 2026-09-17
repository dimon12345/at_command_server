#pragma once

#include <string>

#include "Config.h"


class ArgsParseConfig : public Config {
public:
    ArgsParseConfig(int argc, char **argv);
};
