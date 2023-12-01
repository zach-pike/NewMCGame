#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Multiplayer/Types.hpp"

class Player {
private:
    glm::vec3 position;
    glm::vec3 looking;

    bool socketOpen = true;
    SocketInfo socket;
    PlayerIdentifier identifier;

    std::string name;
public:
    Player(SocketInfo sockInfo, PlayerIdentifier identifier,
           glm::vec3 position, glm::vec3 looking);

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&& p);

    ~Player();

    /**
     * Returns a reference to the looking vector
    */
    glm::vec3& getLookingVectorRef();

    /**
     * Returns a reference to the position vector
    */
    glm::vec3& getPositionRef();

    inline PlayerIdentifier getIdentifier() const { return identifier; }
    inline SocketInfo getSocketInfo() { return socket; }
};