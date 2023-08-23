#include "pluginManager.hpp"
#include "utils/path.hpp"
#include "game.hpp"

#include <iostream>
#include <filesystem>
#include <dlfcn.h>

namespace fs = std::filesystem;

PluginManager::PluginManager(std::string _dir):
    pluginDirectory(getResourcePath(_dir)) {}

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
    auto paths = fs::directory_iterator(pluginDirectory);
    
    // Open them all first
    for (auto path : paths) {
        if (path.is_directory()) continue;

        std::string pluginObjectPath = path.path();

        if (!hasEnding(pluginObjectPath, ".so")) continue;

        void* pluginHandle = dlopen(pluginObjectPath.c_str(), RTLD_NOW);

        if (pluginHandle == nullptr) {
            std::cout << "[PluginManager] Failed to load a plugin!\nPath: " << pluginObjectPath
                      << "\nError message from dlerror(): " << dlerror() << '\n';
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

        std::cout << "[PluginManager] Created plugin " << name << " version "
                  << version.major << '.' << version.minor << '\n';
    }

    std::cout << "[PluginManager] Loaded " << getPluginsLoaded() << " Plugin(s)\n";
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

    std::cout << "[PluginManager] Unloaded " << pluginHandleCount << " Plugin(s)\n";
}

void PluginManager::enablePlugins(Game& game) {
    // Setup all the plugins
    for (auto& plugin : plugins) {
        plugin->setup(game);
    }

    std::cout << "[PluginManager] Enabled " << plugins.size() << " Plugin(s)\n";
    pluginsEnabled = true;
}

void PluginManager::disablePlugins(Game& game) {
    // Cleanup all the plugins
    std::size_t n = plugins.size();
    for (auto& plugin : plugins) {
        plugin->cleanup(game);
    }

    std::cout << "[PluginManager] Disabled " << n << " Plugin(s)\n";
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