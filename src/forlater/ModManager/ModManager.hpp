#pragma once

#include <vector>
#include <memory>
#include <string>
#include <filesystem>

#include "include/IMod.hpp"
#include "glHelpers/utils/logger/logger.hpp"

class Game;

class ModManager {
private:
    std::vector<void*> modHandles;
    std::vector<std::unique_ptr<IMod, ModDestroyerFunction>> mods;

    Logger logger{ "ModManager", Logger::FGColors::GREEN };

    bool modsEnabled = false;
public:
    // The resources folder is the base path for the pluginsDirector
    ModManager();
    ~ModManager();

    /**
     * Load the .so files and create the mod objects
    */
    void loadMods();

    /**
     * Destroy the mods and unload the .so files
    */
    void unloadMods();

    /**
     * Calls all the setup functions
     * @param game The game object to pass to IMod::setup()
    */
    void enableMods(Game& game);

    /**
     * Calls all the cleanup functions
     * @param game The game object to pass to IMod::cleanup()
    */
    void disableMods(Game& game);

    /**
     * Calls IMod::frameUpdate() on every plugin
     * @param game The game object to pass
    */
    void modFrameUpdate(Game& game);

    /**
     * Returns the number of mods loaded
    */
    std::size_t getNModsLoaded() const;
};