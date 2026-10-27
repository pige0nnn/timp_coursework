#pragma once
#include <string>
#include <map>

class ClientDB {
private:
    std::map<std::string, std::string> users;
public:
    void load(const std::string& filename);
    bool userExists(const std::string& login);
    std::string getPassword(const std::string& login);
};
