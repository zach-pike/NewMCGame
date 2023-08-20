#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "player/player.hpp"
#include "world/world.hpp"

class Game {
private:
    // Game vars
    GLFWwindow* game_window;

    Player player;
    World world;
public:
    Game();
    ~Game();

    Player& getPlayer();
    World& getWorld();

    GLFWwindow* getGLFWwindow();

    void gameloop();
};