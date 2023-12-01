#pragma once
#include <thread>

#include "Multiplayer/TSQueue.hpp"
#include "Multiplayer/MessageTypes.hpp"
#include "player/player.hpp"


/*
THEORY FOR GAMESERVER
-------------------------
Gameserver manages tcp connections to clients, manages all texture data for blocks and 
serves that data to the clients to allow for easier mod and block compatibiliy
*/

#include <sys/socket.h>

class GameServer {
private:

private:
    bool networkingThreadRunning = false;
    std::jthread networkingThread;
    std::uint16_t port;

    bool gameUpdateThreadRunning = false;
    std::jthread gameUpdateThread;

    TSQueue<ClientServerMessage> inboundMessages;
    TSQueue<ServerClientMessage> outboundMessages;

    // Game Components
    std::vector<Player> players;

    // TBD...


private:
    void _networkingThread();
    void _gameServerThread();
public:
    GameServer(std::uint16_t _port = 4432);
    ~GameServer();

    void startWorkers();
    void stopWorkers();

    bool workersRunning() const;
};