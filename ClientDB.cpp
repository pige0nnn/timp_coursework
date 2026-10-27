#include "ClientDB.h"
#include <fstream>
#include <stdexcept>

void ClientDB::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open auth DB");
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            users[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }
}

bool ClientDB::userExists(const std::string& login) { 
    return users.find(login) != users.end(); 
}

std::string ClientDB::getPassword(const std::string& login) { 
    if (userExists(login)) return users[login];
    return "";
}
