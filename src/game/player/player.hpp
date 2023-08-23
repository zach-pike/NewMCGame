#pragma once

#include <glm/glm.hpp>

class Game;

class Player {
public:
    struct PlayerSettings {
        float fov;
        bool showDebug;
        bool mouseLookFocus;
    };
private:
    glm::vec3 position;
    glm::vec3 looking;

    PlayerSettings settings;
public:
    Player(glm::vec3 position, glm::vec3 looking, float fov);
    ~Player();

    /**
     * Updates the player (movement, looking, keybinds, etc..)
     * @param game Reference to the Game object
    */
    void updatePlayer(Game& game);

    /**
     * Gets the projection matrix used for the player
     * @param aspect Aspect ratio of the screen
    */
    glm::mat4 getCameraProjectionMatrix(float aspect) const;

    /**
     * Gets the view matrix used for the player
    */
    glm::mat4 getCameraViewMatrix() const;

    /**
     * Gets the View Projection matrix (sameas getCameraViewMatrix()*getCameraProjectionMatrix())
     * @param aspect Aspect ratio of the screen
    */
    glm::mat4 getViewProjection(float aspect) const;

    /**
     * Returns true if the player is requesting the debug lines to be drawn
    */
    bool showingDebug() const;

    /**
     * Returns a reference to the looking vector
    */
    glm::vec3& getLookingVectorRef();

    /**
     * Returns a reference to the position vector
    */
    glm::vec3& getPositionRef();
};