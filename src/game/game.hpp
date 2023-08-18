#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "player/player.hpp"

class Game {
private:
    // Game vars
    GLFWwindow* game_window;

    Player player;
public:
    Game();
    ~Game();

    GLFWwindow* getGLFWwindow();

    void gameloop();
};