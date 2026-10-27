#include "Authenticator.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

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
    return computeSHA224Hash(salt16, password) == storedHash;
}
