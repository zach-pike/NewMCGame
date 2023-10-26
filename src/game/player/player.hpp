#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "world/chunk/block/block.hpp"
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

    // std::vector<Block::BlockType> selectableBlocks = {
    //     Block::BlockType::DIRT,
    //     Block::BlockType::GRASS,
    //     Block::BlockType::STONE,
    //     Block::BlockType::OAK_WOOD,
    //     Block::BlockType::DOUBLE_SLAB,
    //     Block::BlockType::WHOLE_SLAB,
    //     Block::BlockType::BRICKS,
    //     Block::BlockType::TNT,
    //     Block::BlockType::COBBLESTONE,
    //     Block::BlockType::BEDROCK,
    //     Block::BlockType::SAND,
    //     Block::BlockType::GRAVEL
    // };
    // std::vector<Block::BlockType>::iterator selectedBlock = selectableBlocks.begin();
public:
    Player(glm::vec3 position, glm::vec3 looking, float fov);
    ~Player();

    /**
     * Updates the player (movement, looking, keybinds, etc..)
     * @param game Reference to the Game object
    */
    void updatePlayer(Game& game, float deltaTime);

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