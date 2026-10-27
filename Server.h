#pragma once
#include "ClientDB.h"
#include "Authenticator.h"
#include "VectorProcessor.h"
#include "Logger.h"
#include "UserInterface.h"

class Server {
private:
    uint16_t port;
    int serverSocket;
    ClientDB clientDB;
    Logger logger;
    
    void handleClient(int clientSocket);

public:
    Server();
    void start(int argc, const char** argv);
};
