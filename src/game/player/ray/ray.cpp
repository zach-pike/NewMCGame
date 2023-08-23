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
         && world.getBlock(cPos).getBlockType() != Block::BlockType::AIR) {
            world.setBlock(cPos, Block(Block::BlockType::AIR));
            return true;
        }
    }
    return false;
}

bool Ray::tryPlaceBlock(World& game, Block block, float maxDist) {
    return false;
}