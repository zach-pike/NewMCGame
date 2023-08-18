#pragma once

#include <glm/glm.hpp>

class Game;

class Player {
private:
    glm::vec3 position;
    glm::vec3 looking;
public:
    Player(glm::vec3 position, glm::vec3 looking);
    ~Player();

    void updatePlayer(Game& game);

    glm::mat4 getMVPmatrix(float aspect, float fov) const;
};