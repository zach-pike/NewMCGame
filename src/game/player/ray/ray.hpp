#pragma once

#include <glm/glm.hpp>

class World;

class Ray {
private:
    glm::vec3 startPosition;
    glm::vec3 direction;
    float moveScalar;
public:
    // Step the ray forward
    void tryBreakBlock(World& game, float maxDist = 10.f);

    Ray(glm::vec3 _position, glm::vec3 _direction, float _moveScalar = .1);
    ~Ray();
};