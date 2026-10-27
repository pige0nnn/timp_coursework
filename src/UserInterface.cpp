#include "UserInterface.h"
#include <sstream>
#include <iostream>

UserInterface::UserInterface() : desc("Allowed options") {
    desc.add_options()
        ("help,h", "Show help")
        ("auth,a", po::value<std::string>(&params.authFile)->default_value("/etc/vcalc.conf"), "Auth DB file")
        ("log,l", po::value<std::string>(&params.logFile)->default_value("/var/log/vcalc.log"), "Log file")
        ("port,p", po::value<uint16_t>(&params.port)->default_value(33333), "Server port");
}

bool UserInterface::Parser(int argc, const char** argv) {
    if (argc <= 1) {
        return false;
    }

    po::store(po::parse_command_line(argc, argv, desc), vm);
    
    if (vm.count("help")) {
        return false;
    }
    
    po::notify(vm);
    return true;
}

std::string UserInterface::getDescription() {
    std::ostringstream ss;
    ss << desc;
    return ss.str();
}
