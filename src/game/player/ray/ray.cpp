#include "ray.hpp"
#include "../../game.hpp"

Ray::Ray(glm::vec3 _position, glm::vec3 _direction, float _moveScalar):
    startPosition{_position},
    direction{glm::normalize(_direction)},
    moveScalar{_moveScalar} {}

Ray::~Ray() {}

bool Ray::tryBreakBlock(World& world, float maxDist) {
    glm::vec3 cPos = startPosition;
    glm::vec3 moveVec = direction * moveScalar;
    double distTraveled = 0;

    while(distTraveled < maxDist) {
        cPos += moveVec;
        distTraveled += moveScalar;
        // Check if the ray is in world bounds

        if (world.coordinatesInWorld(cPos)
         && world.getBlock(cPos).getBlockId() != 0) {
            world.setBlock(cPos, Block(0));
            return true;
        }
    }
    return false;
}

bool Ray::tryPlaceBlock(World& world, Block block, float maxDist) {
    double distTraveled = 0;

    using namespace glm;

    while(distTraveled < maxDist) {
        auto beforePos = startPosition + direction * glm::vec3(distTraveled - moveScalar);
        auto pos = startPosition + direction * glm::vec3(distTraveled);
        if (!world.coordinatesInWorld(pos)) continue;

        if (world.getBlock(pos).getBlockId() != 0) {
            world.setBlock(beforePos, block);
            return true;
        }

        distTraveled += moveScalar;
    }

    return false;
}