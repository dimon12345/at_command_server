#include <iostream>

#include <argparse.hpp>

#include "Config.h"


namespace {
    const std::string AppName = "AT command server";
    const std::string DefaultDictionaryFilename = "dictionary.txt";

    void parse_arguments(argparse::ArgumentParser &program,
                                int argc, char **argv) {
        program
            .add_argument("-d", "-dictionary_filename")
            .help("dictionary filename")
            .default_value("");
        program.add_argument("-v", "--verbose")
            .help("enable verbose output")
            .default_value(false)
            .implicit_value(true);
        program.parse_args(argc, argv);
    }
}

Config::Config(int argc, char **argv) {
    argparse::ArgumentParser program(AppName);
    ::parse_arguments(program, argc, argv);

    std::string filename = program.get<std::string>("dictionary_filename");
    if (!filename.empty()) {
        dictionary_filename = filename;
    } else {
        dictionary_filename = DefaultDictionaryFilename;
    }

    verbose = program.get<bool>("verbose");
}
