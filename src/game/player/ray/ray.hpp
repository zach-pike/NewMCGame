#pragma once

#include <glm/glm.hpp>

class Game;

class Ray {
private:
    glm::vec3 position;
    glm::vec3 direction;
    float moveScalar;

    float distancedMoved = 0.f;

    // Steps the ray forward
    void step();
public:
    // Step the ray forward
    void tryBreakBlock(Game& game, float maxDist = 10.f);

    Ray(glm::vec3 _position, glm::vec3 _direction, float _moveScalar);
    ~Ray();
};