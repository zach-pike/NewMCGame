#pragma once

#include "pluginManager/include/IPlugin.hpp"
#include "billboardManager/billboard/billboard.hpp"
#include "game.hpp"

#include <memory>

class WorldInfoSign : public IPlugin {
private:
    std::shared_ptr<Billboard> billboard;
public:
    void setup(Game&);
    void frameUpdate(Game&);
    void cleanup(Game&);
    std::string getPluginName();
    Version getPluginVersion();
};

extern "C" {
    IPlugin* create();
    void destroy(IPlugin* plugin);
}