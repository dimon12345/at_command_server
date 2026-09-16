#pragma once

#include <string>

class Config {
public:
    Config();
    Config(int argc, char **argv);

    std::string dictionary_filename;
    bool verbose;
};
