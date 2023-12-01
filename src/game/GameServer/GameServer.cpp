#include "GameServer.hpp"

#include <functional>
#include <span>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils/logger/logger.hpp"
#include "utils/dotenv.h"

bool setSocketBlockingEnabled(int fd, bool blocking) {
   if (fd < 0) return false;

   int flags = fcntl(fd, F_GETFL, 0);
   if (flags == -1) return false;
   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
}

void GameServer::_networkingThread() {
    networkingThreadRunning = true;

    Logger logger("Networking Thread", Logger::FGColors::CYAN);
    // Networking initialize

    int gameServerFD = socket(AF_INET, SOCK_STREAM, 0);

    if (gameServerFD < 0) {
        logger.error("Failed to create TCP socket!");
        throw std::runtime_error("Failed to create TCP socket!");
    }

    int opt = 1;
    if(setsockopt(gameServerFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        logger.error("setsockopt Error!");
        throw std::runtime_error("setsockopt Error!");
    }

    // async goodness
    setSocketBlockingEnabled(gameServerFD, false);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(gameServerFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        logger.error("bind Error!");
        throw std::runtime_error("bind Error!");
    }

    if (listen(gameServerFD, 5) < 0) {
        logger.error("listen Error!");
        throw std::runtime_error("listen Error!");
    }

    // Networking loop
    socklen_t sockaddrLen = sizeof(sockaddr);

    srand(time(NULL));

    while(networkingThreadRunning) {
        sockaddr connectingSocketAddress;
        int connectingSocketFd = accept(gameServerFD, &connectingSocketAddress, &sockaddrLen);

        // This executes if we have a new connection
        if (connectingSocketFd >= 0) {
            logger.info("New connection");
            // Generate random uint64_t
            PlayerIdentifier uid;

            // Should generate 64 bit identifier
            *(int*)&uid = rand();
            *((int*)(&uid) + 1) = rand();

            players.push_back(Player(
                SocketInfo{
                    .sockfd = connectingSocketFd,
                    .socketAddress = connectingSocketAddress
                },
                uid,
                glm::vec3(0, 0, 0),
                glm::vec3(1, 0, 0)
            ));

            // Write player uid to connecting device
            write(connectingSocketFd, &uid, sizeof(PlayerIdentifier));
        }


        // Check if we have any pending outbound messages
        // if (outboundMessages.messageAvailable()) {
        //     // We need to pack the message
        //     auto msg = outboundMessages.getMessage();
            
        //     // Do the sendy stuff here
        // }

        // Try to read a packet from each connection
        for (auto& player : players) {
            int clientSocket = player.getSocketInfo().sockfd;

            std::uint64_t packetSize;
            std::uint8_t countRead = 0;
            while (countRead < sizeof(std::uint64_t)) {
                countRead += read(clientSocket, (&packetSize + countRead), sizeof(std::uint64_t) - countRead);
            }
            // Try to read a packet
            std::vector<std::uint8_t> packetData;

            while(packetData.size() < packetSize) {
                std::uint8_t readBuf[1024];
                std::size_t ammount = read(clientSocket, readBuf, 1024);

                auto part = std::span<std::uint8_t>(readBuf, readBuf + ammount);
                packetData.insert(packetData.end(), part.begin(), part.end());
            }

            // Parse the message
            ClientServerMessage msg;
            msg.id = *(PlayerIdentifier*)(packetData.data());
            msg.msgType = *(ClientServerMessageTypes*)(packetData.data() + sizeof(PlayerIdentifier));

            std::size_t contentStart = sizeof(PlayerIdentifier) + sizeof(ClientServerMessageTypes);
            msg.messageContent = std::vector<std::uint8_t>(packetData.begin() + contentStart, packetData.end());

            // Add parsed message
            inboundMessages.addMessage(msg);
        }
    }

    // Loop thru all the connections and close them
    for (auto& player : players) close(player.getSocketInfo().sockfd);
    players.clear();

    // Close server
    close(gameServerFD);
}

void GameServer::_gameServerThread() {
    gameUpdateThreadRunning = true;
    // Game init (worldgen etc)

    while (gameUpdateThreadRunning) {
        // Game update
    }
}

GameServer::GameServer(std::uint16_t _pt):
    port(_pt)
{
    dotenv::init();
}
GameServer::~GameServer() {}

void GameServer::startWorkers() {
    // wakey wakey time for tcp server thread and networking
    networkingThread = std::jthread(std::bind(&GameServer::_networkingThread, this));
    gameUpdateThread = std::jthread(std::bind(&GameServer::_gameServerThread, this));
}

void GameServer::stopWorkers() {
    gameUpdateThreadRunning = false;
    networkingThreadRunning = false;
}

bool GameServer::workersRunning() const {
    return gameUpdateThreadRunning || networkingThreadRunning;
}