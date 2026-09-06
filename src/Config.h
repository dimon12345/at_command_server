#pragma once

#include <string>

class Config {
public:
    Config(int argc, char **argv);
    std::string dictionary_filename;
    bool verbose;
};
