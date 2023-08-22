#pragma once

#include <vector>
#include <memory>
#include <string>

#include "include/IPlugin.hpp"

class Game;

class PluginManager {
private:
    std::string pluginDirectory;

    std::vector<void*> pluginHandles;
    std::vector<std::unique_ptr<IPlugin, PluginDestroyerFunction>> plugins;


    bool pluginsEnabled = false;
public:
    // The resources folder is the base path for the pluginsDirector
    PluginManager(std::string pluginsDirectory = "cplugins/");
    ~PluginManager();

    void loadPlugins();
    void unloadPlugins();

    void enablePlugins(Game&);
    void disablePlugins(Game&);

    void pluginFrameUpdate(Game&);

    std::size_t getPluginsLoaded() const;
};