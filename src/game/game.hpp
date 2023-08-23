#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "player/player.hpp"
#include "world/world.hpp"
#include "pluginManager/pluginManager.hpp"
#include "billboardManager/billboardManager.hpp"

#include <memory>
class Game {
private:
    // OpenGL stuff
    GLuint vertexArrayID;

    // Game vars
    GLFWwindow* gameWindow;

    Player player;
    World world;

    std::int64_t fps = 1.f / 30.f * 1000000.f;

    PluginManager pluginManager;

    // Gets initialized after opengl in constructor
    BillboardManager billboardManager;

    int windowWidth, windowHeight;
public:
    Game();
    ~Game();

    Player& getPlayer();
    World& getWorld();
    PluginManager& getPluginManager();
    BillboardManager& getBillboardManager();

    GLFWwindow* getGLFWwindow();

    void gfxInit();

    void gameLoop();

    int getWindowWidth() const;
    int getWindowHeight() const;
};