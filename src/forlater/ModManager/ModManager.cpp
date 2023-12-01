#include "ModManager.hpp"
#include "game.hpp"
#include "glHelpers/utils/getEnvVar/getEnvVar.hpp"

#include <iostream>
#include <filesystem>
#include <dlfcn.h>

namespace fs = std::filesystem;

ModManager::ModManager() {}

ModManager::~ModManager() {
    if (getNModsLoaded() != 0) {
        unloadMods();
    }
}

bool hasEnding(const std::string& fullString, const std::string& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}


void ModManager::loadMods() {
    if (modHandles.empty() != true) throw std::runtime_error("Plugin manager already has loaded plugins!");

    // Get directory iterator for the mods folder
    auto paths = fs::directory_iterator(fs::absolute(getEnvironmentVar("MODS_FOLDER")));
    
    // Open them all first
    for (auto path : paths) {
        if (path.is_directory()) continue;

        std::string modObjectPath = path.path();

        // if file isn't a .so skip it
        if (!hasEnding(modObjectPath, ".so")) continue;

        void* modHandle = dlopen(modObjectPath.c_str(), RTLD_NOW);

        if (modHandle == nullptr) {
            logger.warn("Failed to load a mod!\nPath: " + modObjectPath + "\nError message from dlerror(): " + dlerror());
        } else {
            modHandles.push_back(modHandle);
        }
    }

    // Load all the mod objects
    for (void* modHandle : modHandles) {
        // Create and destroy functions for modules
        auto createFn = (FP<IMod*>)dlsym(modHandle, "create");
        auto destroyFn = (FP<void, IMod*>)dlsym(modHandle, "destroy");

        // Create the mod
        auto mod = std::unique_ptr<IMod, ModDestroyerFunction>(createFn(), destroyFn);
        std::string name = mod->getModName();
        IMod::Version version = mod->getModVersion();
        mods.push_back(std::move(mod));

        logger.info("Loaded mod " + name + " version " + std::to_string(version.major) + "." + std::to_string(version.minor));
    }

    logger.info("Loaded " + std::to_string(getNModsLoaded()) + " Mod(s)");
}

void ModManager::unloadMods() {
    // If the plugin objects are still enabled then clean then up before we delete the handles
    if (modsEnabled) throw std::runtime_error("Plugins still enabled!");

    // This will call the destructor on all the std::unique_ptr's which will
    // call the destructor on all the Plugins
    mods.clear();

    // Close the handles
    std::size_t modHandleCount = modHandles.size();
    for (void* handle : modHandles) {
        dlclose(handle);
    }
    modHandles.clear();

    logger.info("Unloaded " + std::to_string(modHandleCount) + " Mod(s)");
}

void ModManager::enableMods(Game& game) {
    // Setup all the plugins
    for (auto& mod : mods) {
        mod->setup(game);
    }

    logger.info("Enabled " + std::to_string(mods.size()) + " Mods(s)");
    modsEnabled = true;
}

void ModManager::disableMods(Game& game) {
    // Cleanup all the mods
    std::size_t n = mods.size();
    for (auto& mod : mods) {
        mod->cleanup(game);
    }

    logger.info("Disabled " + std::to_string(n) + " Mod(s)");

    modsEnabled = false;
}

void ModManager::modFrameUpdate(Game& game) {
    for (auto& mod : mods) {
        mod->frameUpdate(game);
    }
}

std::size_t ModManager::getNModsLoaded() const {
    return mods.size();
}