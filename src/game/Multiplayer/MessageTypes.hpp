#pragma once

#include <cstdint>
#include <vector>

#include "Types.hpp"

enum class ServerClientMessageTypes : std::uint8_t {
    HEARTBEAT = 0,
};

enum class ClientServerMessageTypes : std::uint8_t {
    HEARTBEAT_RESP = 0
};

struct ClientServerMessage {
    PlayerIdentifier id;
    ClientServerMessageTypes msgType;
    std::vector<std::uint8_t> messageContent;
};

struct ServerClientMessage {
    ClientServerMessageTypes msgType;
    std::vector<std::uint8_t> messageContent;

    inline std::size_t getSize() { return sizeof(msgType) + messageContent.size(); }
};