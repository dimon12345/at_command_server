#include <iostream>

#include "Config.h"

namespace {
    const std::string DefaultDictionaryFilename = "dictionary.txt";
}

Config::Config() : dictionary_filename(DefaultDictionaryFilename), verbose(false) {
}
