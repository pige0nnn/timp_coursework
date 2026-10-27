#include "Logger.h"
#include <iostream>
#include <ctime>
#include <stdexcept>

Logger::Logger(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Warning: Cannot open log file " << filename << ". Logging to console." << std::endl;
    }
}

Logger::~Logger() { if (logFile.is_open()) logFile.close(); }

void Logger::log(const std::string& message, int severity) {
    std::lock_guard<std::mutex> lock(mtx);
    time_t now = time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    std::string sevStr = (severity == 2) ? "CRITICAL" : (severity == 1 ? "WARNING" : "INFO");
    std::string logLine = std::string(buf) + " [" + sevStr + "] " + message;
    
    if (logFile.is_open()) {
        logFile << logLine << std::endl;
    } else {
        std::cerr << logLine << std::endl;
    }
    
    if (severity == 2) {
        std::cerr << "Critical error: " << message << std::endl;
        exit(1); 
    }
}
