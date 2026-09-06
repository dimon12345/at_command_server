#include <list>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>

#include "Config.h"
#include "TestAtCommandResponseSender.h"
#include "server/AtCommandServerState.h"
#include "server/AtCommandExecutor.h"

namespace {
    struct TestData {
        std::string data_name;
        std::list<std::string> commands;
        std::string response;
    };

    std::list<std::string> loadCommands(const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open '" + filename + "'");
        }

        std::list<std::string> result;
        std::string line;
        while (std::getline(file, line)) {
            if (line.size() > 0 && line[line.size()-1] == '\r') {
                result.push_back(line.substr(0, line.size() - 1));
            } else {
                result.push_back(line);
            }
        }
        return result;
    }

    std::string loadResponse(const std::string &filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open '" + filename + "'");
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }

    std::list <TestData> getAllTestData() {
        std::vector<std::string> test_inputs;
        for (const auto &entry: std::filesystem::directory_iterator("test_data")) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                std::string test_name = entry.path().stem().string();
                if (test_name.ends_with("_session")) {
                    continue;
                }
                std::string session_filename = "test_data/" + test_name + "_session.txt";
                if (std::filesystem::exists(session_filename)) {
                    test_inputs.push_back(test_name);
                } else {
                    throw std::runtime_error("session file '" + session_filename + "' not found");
                }
            }
        }

        std::sort(test_inputs.begin(), test_inputs.end());

        std::list <TestData> result;
        for (auto test_name: test_inputs) {
            result.push_back(TestData{
                test_name,
                loadCommands("test_data/" + test_name + ".txt"),
                loadResponse("test_data/" + test_name + "_session.txt")
            });
        }
        return result;
    }
}


TEST(AtCommandExecutorTest, TestDataTest) {
    int argc = 1;
    char *argv[] = {const_cast<char *>("test"), nullptr};
    Config config(argc, argv);
    config.verbose = true;

    auto all_test_data = getAllTestData();
    ASSERT_FALSE(all_test_data.empty());

    for( auto test_data: all_test_data) {
        const std::string &data_name = test_data.data_name;
        if (config.verbose) {
            std::cout << std::endl << "Start '" << data_name << "' test:" << std::endl;
        }

        TestAtCommandResponseSender sender(config);
        AtCommandServerState state;
        AtCommandExecutor executor(config, state, sender);

        for (auto command: test_data.commands ) {
            sender.addCommand(command);

            if (command.size() == 0 || command[0] == '#') {
                sender.addCommand("\r\n");
                continue;
            }

            ASSERT_EQ(command[0], 'A') << "'A' char check for " << data_name;
            ASSERT_EQ(command[1], 'T') << "'T' char check for " << data_name;
            ASSERT_TRUE(executor.execute(command.substr(2)))
                    << "execute() check for '" << data_name << "' data test failed";
        }
        ASSERT_EQ(sender.getSession(), test_data.response)
            << "equality check for '" << data_name << "' data test failed";
        if (config.verbose) {
            std::cout << "'" << data_name << "' test successful passed" << std::endl;
        }
    }

    std::cout << all_test_data.size() << " test_data/* tests checked successful" << std::endl;
}
