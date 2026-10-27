#include "Server.h"
#include <iostream>

int main(int argc, const char** argv) {
    try {
        Server server;
        server.start(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
