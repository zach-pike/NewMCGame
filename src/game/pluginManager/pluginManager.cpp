#include "pluginManager.hpp"
#include "game.hpp"
#include "glHelpers/utils/getEnvVar/getEnvVar.hpp"

#include <iostream>
#include <filesystem>
#include <dlfcn.h>

namespace fs = std::filesystem;

PluginManager::PluginManager() {}

PluginManager::~PluginManager() {
    if (getPluginsLoaded() != 0) {
        unloadPlugins();
    }
}

bool hasEnding(const std::string& fullString, const std::string& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}


void PluginManager::loadPlugins() {
    if (pluginHandles.empty() != true) throw std::runtime_error("Plugin manager already has loaded plugins!");

    // Get directory iterator for the plugins folder
    auto paths = fs::directory_iterator(fs::absolute(getEnvironmentVar("PLUGINS_FOLDER")));
    
    // Open them all first
    for (auto path : paths) {
        if (path.is_directory()) continue;

        std::string pluginObjectPath = path.path();

        if (!hasEnding(pluginObjectPath, ".so")) continue;

        void* pluginHandle = dlopen(pluginObjectPath.c_str(), RTLD_NOW);

        if (pluginHandle == nullptr) {
            logger.warn("Failed to load a plugin!\nPath: " + pluginObjectPath + "\nError message from dlerror(): " + dlerror());
        } else {
            pluginHandles.push_back(pluginHandle);
        }
    }

    // Load all the plugin objects
    for (void* pluginHandle : pluginHandles) {
        // Create and destroy functions for modules
        auto createFn = (FP<IPlugin*>)dlsym(pluginHandle, "create");
        auto destroyFn = (FP<void, IPlugin*>)dlsym(pluginHandle, "destroy");

        // Create the plugin
        auto plugin = std::unique_ptr<IPlugin, PluginDestroyerFunction>(createFn(), destroyFn);
        std::string name = plugin->getPluginName();
        IPlugin::Version version = plugin->getPluginVersion();
        plugins.push_back(std::move(plugin));

        logger.info("Loaded plugin " + name + " version " + std::to_string(version.major) + "." + std::to_string(version.minor));
    }

    logger.info("Loaded " + std::to_string(getPluginsLoaded()) + " Plugin(s)");
}

void PluginManager::unloadPlugins() {
    // If the plugin objects are still enabled then clean then up before we delete the handles
    if (pluginsEnabled) throw std::runtime_error("Plugins still enabled!");

    // This will call the destructor on all the std::unique_ptr's which will
    // call the destructor on all the Plugins
    plugins.clear();

    // Close the handles
    std::size_t pluginHandleCount = getPluginsLoaded();
    for (void* handle : pluginHandles) {
        dlclose(handle);
    }
    pluginHandles.clear();

    logger.info("Unloaded " + std::to_string(pluginHandleCount) + " Plugin(s)");
}

void PluginManager::enablePlugins(Game& game) {
    // Setup all the plugins
    for (auto& plugin : plugins) {
        plugin->setup(game);
    }

    logger.info("Enabled " + std::to_string(plugins.size()) + " Plugin(s)");
    pluginsEnabled = true;
}

void PluginManager::disablePlugins(Game& game) {
    // Cleanup all the plugins
    std::size_t n = plugins.size();
    for (auto& plugin : plugins) {
        plugin->cleanup(game);
    }

    logger.info("Disabled " + std::to_string(n) + " Plugin(s)");

    pluginsEnabled = false;
}

void PluginManager::pluginFrameUpdate(Game& game) {
    for (auto& plugin : plugins) {
        plugin->frameUpdate(game);
    }
}

std::size_t PluginManager::getPluginsLoaded() const {
    return pluginHandles.size();
}