#include <iostream>

#include "Config.h"
#include "server/AtCommandServer.h"


int main(int argc, char **argv) {
    try {
        Config config(argc, argv);

        AtCommandServer server(config);
        return server.run();
    } catch (const std::system_error& e) {
        std::cerr << "System error: " << e.what() << " (код: " << e.code() << ")\n";
        return 1;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
