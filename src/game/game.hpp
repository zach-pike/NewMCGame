#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "player/player.hpp"
#include "world/world.hpp"

class Game {
private:
    // OpenGL stuff
    GLuint vertexArrayID;
    GLuint worldShader, lineDebugShader;
    GLuint textureAtlas;

    // Game vars
    GLFWwindow* gameWindow;

    Player player;
    World world;

    std::int64_t fps = 1.f / 30.f * 1000000.f;
public:
    Game();
    ~Game();

    Player& getPlayer();
    World& getWorld();

    GLFWwindow* getGLFWwindow();

    void gfxInit();

    void gameLoop();
};