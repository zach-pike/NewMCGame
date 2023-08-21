#pragma once

#include <glm/glm.hpp>

class Game;

class Player {
public:
    struct PlayerSettings {
        float fov;
        bool showDebug;
    };
private:
    glm::vec3 position;
    glm::vec3 looking;

    PlayerSettings settings;
public:
    Player(glm::vec3 position, glm::vec3 looking, float fov);
    ~Player();

    bool showingDebug() const;

    void updatePlayer(Game& game);

    glm::mat4 getMVPmatrix(float aspect) const;
};