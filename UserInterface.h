#pragma once
#include <string>
#include <cstdint>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct Params {
    uint16_t port;
    std::string authFile;
    std::string logFile;
};

class UserInterface {
private:
    po::options_description desc;
    po::variables_map vm;
    Params params;

public:
    UserInterface();
    bool Parser(int argc, const char** argv);
    std::string getDescription();
    Params getParams() { return params; }
};
