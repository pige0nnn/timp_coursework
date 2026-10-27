#include "ClientDB.h"
#include <fstream>
#include <stdexcept>

void ClientDB::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open auth DB");
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string login = line.substr(0, pos);
            std::string password = line.substr(pos + 1);
            
            while (!password.empty() && (password.back() == ' ' || password.back() == '\t')) password.pop_back();
            while (!login.empty() && (login.back() == ' ' || login.back() == '\t')) login.pop_back();
            
            if (!login.empty()) {
                users[login] = password;
            }
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
