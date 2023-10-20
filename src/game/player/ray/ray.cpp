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
         && world.getBlock(cPos).getBlockType() != 0) {
            world.setBlock(cPos, Block(0));
            return true;
        }
    }
    return false;
}

bool Ray::tryPlaceBlock(World& world, Block block, float maxDist) {
    glm::vec3 cPos = startPosition;
    glm::vec3 moveVec = direction * moveScalar;
    double distTraveled = 0;

    using namespace glm;

    while(distTraveled < maxDist) {
        cPos += moveVec;
        distTraveled += moveScalar;
        if (!world.coordinatesInWorld(cPos)) continue;

        // Check directions
        // x direction
        auto xPosCheck = cPos + vec3(moveVec.x, 0.f, 0.f);
        auto yPosCheck = cPos + vec3(0.f, moveVec.y, 0.f);
        auto zPosCheck = cPos + vec3(0.f, 0.f, moveVec.z);

        if (world.getBlock(xPosCheck).getBlockType() != 0) {
            // If moveVec.x is negative then the block face is the Positive X face
            // and that holds up for the opposite as well
            if (moveVec.x > 0.f) {
                // x - 1 block position
                auto a = xPosCheck - vec3(1, 0, 0);
                if (world.coordinatesInWorld(a)) world.setBlock(a, block);
                else return false;
            } else {
                // x + 1 block position
                auto a = xPosCheck + vec3(1, 0, 0);
                if (world.coordinatesInWorld(a)) world.setBlock(a, block);
                else return false;
            }

            return true;
        } else if (world.getBlock(yPosCheck).getBlockType() != 0) {
            // If moveVec.y is negative then the block face is the Positive Y face
            // and that holds up for the opposite as well
            if (moveVec.y > 0.f) {
                // y - 1 block position
                auto a = yPosCheck - vec3(0, 1, 0);
                if (world.coordinatesInWorld(a)) world.setBlock(a, block);
                else return false;
            } else {
                // y + 1 block position
                auto a = yPosCheck + vec3(0, 1, 0);
                if (world.coordinatesInWorld(a)) world.setBlock(a, block);
                else return false;
            }

            return true;
        } else if (world.getBlock(zPosCheck).getBlockType() != 0) {
            // If moveVec.z is negative then the block face is the Positive Z face
            // and that holds up for the opposite as well
            if (moveVec.z > 0.f) {
                // z - 1 block position
                auto a = zPosCheck - vec3(0, 0, 1);
                if (world.coordinatesInWorld(a)) world.setBlock(a, block);
                else return false;
            } else {
                // z + 1 block position
                auto a = zPosCheck + vec3(0, 0, 1);
                if (world.coordinatesInWorld(a)) world.setBlock(a, block);
                else return false;
            }

            return true;
        }

        cPos += moveVec;
        distTraveled += moveScalar;
    }

    return false;
}