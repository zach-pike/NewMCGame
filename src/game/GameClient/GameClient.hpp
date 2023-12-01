#pragma once

#include <string>
#include <cstdint>
#include <thread>
#include <vector>

#include "Multiplayer/TSQueue.hpp"
#include "Multiplayer/MessageTypes.hpp"

/*
THEORY FOR GAMECLIENT
-------------------------

The gameclient should be basicially the opengl part of the game, it can request
world data from the server and can manage chunk meshes.


*/

class GameClient {
private:
    struct ServerMessage {
        ServerClientMessageTypes msgType;
        std::vector<std::uint8_t> content;
    };
private:
    std::string host;
    std::uint16_t port;

    bool networkingThreadRunning = false;
    std::jthread networkingThread;

    TSQueue<ClientServerMessage> inboundMessages;
    TSQueue<ServerClientMessage> outboundMessages;

    void _networkingThread();

    bool gfxThreadRunning = false;
    std::jthread gfxThread;

    void _gfxThread();
    
    std::string playerName;
public:
    GameClient();
    ~GameClient();

    void startNetworking();
    void startupGfx();
};