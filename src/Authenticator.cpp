#include "Authenticator.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <algorithm>
#include <cctype>

std::string Authenticator::computeSHA224Hash(const std::string& salt16, const std::string& password) {
    CryptoPP::SHA224 hash;
    std::string digest;
    std::string input = salt16 + password;
    
    CryptoPP::StringSource ss(input, true,
        new CryptoPP::HashFilter(hash, 
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest), false)));
    return digest;
}

bool Authenticator::verifyHash(const std::string& salt16, const std::string& password, const std::string& storedHash) {
    std::string computedHash = computeSHA224Hash(salt16, password);
    
    std::string lowerStored = storedHash;
    std::transform(lowerStored.begin(), lowerStored.end(), lowerStored.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    
    std::string lowerComputed = computedHash;
    std::transform(lowerComputed.begin(), lowerComputed.end(), lowerComputed.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    
    return lowerStored == lowerComputed;
}
