#pragma once

#include <vector>
#include <memory>
#include <string>
#include <filesystem>

#include "include/IPlugin.hpp"
#include "glHelpers/utils/logger/logger.hpp"

class Game;

class PluginManager {
private:
    std::vector<void*> pluginHandles;
    std::vector<std::unique_ptr<IPlugin, PluginDestroyerFunction>> plugins;

    Logger logger{ "PluginManager", Logger::FGColors::GREEN };

    bool pluginsEnabled = false;
public:
    // The resources folder is the base path for the pluginsDirector
    PluginManager();
    ~PluginManager();

    /**
     * Load the .so files and create the plugin objects
    */
    void loadPlugins();

    /**
     * Destroy the plugins and unload the .so files
    */
    void unloadPlugins();

    /**
     * Calls all the setup functions
     * @param game The game object to pass to IPlugin::setup()
    */
    void enablePlugins(Game& game);

    /**
     * Calls all the cleanup functions
     * @param game The game object to pass to IPlugin::cleanup()
    */
    void disablePlugins(Game& game);

    /**
     * Calls IPlugin::frameUpdate() on every plugin
     * @param game The game object to pass
    */
    void pluginFrameUpdate(Game& game);

    /**
     * Returns the number of plugins loaded
    */
    std::size_t getPluginsLoaded() const;
};