#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Game {
private:
    // Game vars
    GLFWwindow* game_window;

public:
    Game();
    ~Game();

    void gameloop();
};