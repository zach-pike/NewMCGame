#pragma once

#include <sys/socket.h>

using PlayerIdentifier = std::uint64_t;

struct SocketInfo {
    int sockfd;
    sockaddr socketAddress;
};