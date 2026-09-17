#pragma once

#include <string>

class Config {
public:
    Config();

    std::string dictionary_filename;
    bool verbose;
};
