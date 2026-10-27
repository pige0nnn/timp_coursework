#pragma once
#include <string>

class Authenticator {
public:
    static std::string computeSHA224Hash(const std::string& salt16, const std::string& password);
    static bool verifyHash(const std::string& salt16, const std::string& password, const std::string& storedHash);
};
