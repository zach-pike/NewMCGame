#pragma once

#include <glm/glm.hpp>
#include "world/chunk/block/block.hpp"

class World;

class Ray {
private:
    glm::vec3 startPosition;
    glm::vec3 direction;
    float moveScalar;
public:
    Ray(glm::vec3 _position, glm::vec3 _direction, float _moveScalar = .1);
    ~Ray();

    /**
     * Try to break the block in the world
     * @param world A reference to the world object to modify
     * @param maxDist The max reach distance
    */
    bool tryBreakBlock(World& world, float maxDist = 10.f);

    /**
     * Try to place the block in the world
     * @param world A reference to the world object to modify
     * @param block The block to place
     * @param maxDist The maximum reach distance 
    */
    bool tryPlaceBlock(World& world, Block block, float maxDist = 10.f);
};