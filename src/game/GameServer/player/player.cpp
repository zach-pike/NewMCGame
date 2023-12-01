#include "player.hpp"

#include "Multiplayer/MessageTypes.hpp"

#include <stdio.h>
#include <string.h>

Player::Player(SocketInfo sockInfo, PlayerIdentifier id,
           glm::vec3 pos, glm::vec3 look):
    socket(sockInfo),
    identifier(id),
    position(pos),
    looking(look) {}

Player::~Player() {
    // If player get deleted then their connection should close
    close(socket.sockfd);
}

Player::Player(Player&& p) {
    socket = p.socket;
    identifier = p.identifier;
    position = p.position;
    looking = p.looking;
    p.socketOpen = false;
    p.socket.sockfd = -1;
    memset(&p.socket.socketAddress, 0, sizeof(p.socket.socketAddress));
}

glm::vec3& Player::getLookingVectorRef() {
    return looking;
}

glm::vec3& Player::getPositionRef() {
    return position;
}