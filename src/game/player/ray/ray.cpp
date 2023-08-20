#include "ray.hpp"
#include "../../game.hpp"

Ray::Ray(glm::vec3 _position, glm::vec3 _direction, float _moveScalar):
    position{_position},
    direction{glm::normalize(_direction)},
    moveScalar{_moveScalar} {}

Ray::~Ray() {}

void Ray::step() {
    auto a = direction * moveScalar;
    position += a;
    distancedMoved += glm::length(a);
}

void Ray::tryBreakBlock(Game& game, float maxDist) {
    while(distancedMoved < maxDist) {
        step();
        // Check if the ray is in world bounds

        if (game.getWorld().coordinatesInWorld(position)
         && game.getWorld().getBlock(position).getBlockType() != Block::BlockType::AIR) {

            game.getWorld().setBlock(position, Block(Block::BlockType::AIR));
            
            break;
        }
    }
}