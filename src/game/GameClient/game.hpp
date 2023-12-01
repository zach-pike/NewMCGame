#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "player/player.hpp"
#include "world/world.hpp"
#include "ModManager/ModManager.hpp"
#include "billboardManager/billboardManager.hpp"
#include "old/hudTextManager/hudTextManager.hpp"

#include "ScreenOverlays/ScreenOverlays.hpp"

#include <memory>
class Game {
private:
    // OpenGL stuff
    GLuint vertexArrayID;

    // Game vars
    GLFWwindow* gameWindow;

    Player player;
    World world;
    ModManager modManager;
    BillboardManager billboardManager;
    HUDTextManager hudTextManager;

    ScreenOverlays screenOverlays;

    int windowWidth, windowHeight;

    float maxViewDist = 250.f;
public:
    Game();
    ~Game();

    /**
     * Initializes all OpenGL related stuff
    */
    void gfxInit();

    /**
     * Do the gameLoop (returns when game quits)
    */
    void gameLoop();

    /**
     * Get a reference to the Player object
    */
    Player& getPlayerRef();

    /**
     * Get a reference to the world
    */
    World& getWorldRef();

    /**
     * Get a reference to the ModManager
    */
    ModManager& getModManagerRef();

    /**
     * Get a reference to the BillboardManager
    */
    BillboardManager& getBillboardManagerRef();

    /**
     * Get a reference to the HUDTextManager
     * @deprecated
    */
    HUDTextManager& getHUDTextManagerRef();

    /**
     * Returns the GLFWwindow handle. (Mod API users probably don't use this unless you know what your doing)
    */
    GLFWwindow* getGLFWwindow();

    /**
     * Returns the window width
    */
    int getWindowWidth() const;

    /**
     * Returns the window height
    */
    int getWindowHeight() const;
};