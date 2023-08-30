#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "player/player.hpp"
#include "world/world.hpp"
#include "pluginManager/pluginManager.hpp"
#include "billboardManager/billboardManager.hpp"
#include "hudTextManager/hudTextManager.hpp"

#include <memory>
class Game {
private:
    // OpenGL stuff
    GLuint vertexArrayID;

    // Game vars
    GLFWwindow* gameWindow;

    Player player;
    World world;
    PluginManager pluginManager;
    BillboardManager billboardManager;
    HUDTextManager hudTextManager;

    int windowWidth, windowHeight;
public:
    Game();
    ~Game();

    /**
     * Get a reference to the Player object
    */
    Player& getPlayerRef();

    /**
     * Get a reference to the world
    */
    World& getWorldRef();

    /**
     * Get a reference to the PluginManager
    */
    PluginManager& getPluginManagerRef();

    /**
     * Get a reference to the BillboardManager
    */
    BillboardManager& getBillboardManagerRef();

    /**
     * Get a reference to the HUDTextManager
    */
    HUDTextManager& getHUDTextManagerRef();

    /**
     * Returns the GLFWwindow handle.
    */
    GLFWwindow* getGLFWwindow();

    /**
     * Initializes all OpenGL related stuff
    */
    void gfxInit();

    /**
     * Do the gameLoop (returns when game quits)
    */
    void gameLoop();

    /**
     * Returns the window width
    */
    int getWindowWidth() const;

    /**
     * Returns the window height
    */
    int getWindowHeight() const;
};