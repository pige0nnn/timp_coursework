#include "UserInterface.h"
#include <sstream>
#include <iostream>

UserInterface::UserInterface() : desc("Allowed options") {
    desc.add_options()
        ("help,h", "Show help")
        ("port,p", po::value<uint16_t>(&params.port)->default_value(33333), "Server port")
        ("auth,a", po::value<std::string>(&params.authFile)->default_value("auth_base.txt"), "Auth DB file")
        ("log,l", po::value<std::string>(&params.logFile)->default_value("server.log"), "Log file");
}

bool UserInterface::Parser(int argc, const char** argv) {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) return false;
    po::notify(vm);
    return true;
}

std::string UserInterface::getDescription() {
    std::ostringstream ss;
    ss << desc;
    return ss.str();
}
