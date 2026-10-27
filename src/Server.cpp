#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>

Server::Server() : port(33333), serverSocket(-1), logger("server.log") {}

void Server::start(int argc, const char** argv) {
    UserInterface ui;
    if (!ui.Parser(argc, argv)) {
        std::cout << ui.getDescription() << std::endl;
        return;
    }
    
    Params params = ui.getParams();
    port = params.port;
    
    clientDB.load(params.authFile);
    logger.log("Сервер запущен", 0);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        logger.log("Ошибка создания сокета", 2);
        return;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&addr, sizeof(addr)) < 0) {
        logger.log("Ошибка bind", 2);
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 5) < 0) {
        logger.log("Ошибка listen", 2);
        close(serverSocket);
        return;
    }

    std::cout << "Сервер запущен на порту " << port << "..." << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) continue;
        handleClient(clientSocket);
        close(clientSocket);
    }
}

void Server::handleClient(int clientSocket) {
    uint32_t len = 0;
    if (recv(clientSocket, &len, sizeof(len), MSG_WAITALL) != sizeof(len)) return;
    
    std::string msg(len, '\0');
    if (recv(clientSocket, &msg[0], len, MSG_WAITALL) != (ssize_t)len) return;

    if (msg.length() < 16 + 56) {
        send(clientSocket, "ERR", 3, 0);
        return;
    }
    
    std::string login = msg.substr(0, msg.length() - 16 - 56);
    std::string salt16 = msg.substr(msg.length() - 16 - 56, 16);
    std::string hash = msg.substr(msg.length() - 56);

    if (!clientDB.userExists(login) || 
        !Authenticator::verifyHash(salt16, clientDB.getPassword(login), hash)) {
        logger.log("Ошибка аутентификации для " + login, 1);
        send(clientSocket, "ERR", 3, 0);
        return;
    }

    send(clientSocket, "OK", 2, 0);
    logger.log("Клиент " + login + " аутентифицирован", 0);

    uint32_t n = 0;
    if (recv(clientSocket, &n, sizeof(n), MSG_WAITALL) != sizeof(n)) return;

    send(clientSocket, &n, sizeof(n), 0);

    for (uint32_t i = 0; i < n; ++i) {
        uint32_t s = 0;
        if (recv(clientSocket, &s, sizeof(s), MSG_WAITALL) != sizeof(s)) break;
        
        std::vector<double> vec(s);
        if (recv(clientSocket, vec.data(), s * sizeof(double), MSG_WAITALL) != (ssize_t)(s * sizeof(double))) break;
        
        double result = VectorProcessor::multiply(vec);
        send(clientSocket, &result, sizeof(result), 0);
    }
    
    logger.log("Сеанс завершён", 0);
}
