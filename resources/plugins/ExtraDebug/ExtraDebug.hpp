#pragma once

#include "pluginManager/include/IPlugin.hpp"
#include "game.hpp"

#include "hudTextManager/hudTextItem/hudTextItem.hpp"

#include <memory>

class ExtraDebug : public IPlugin {
private:
    std::shared_ptr<HUDTextItem> debugText = nullptr;
public:
    void setup(Game& game);
    void frameUpdate(Game& game);
    void cleanup(Game& game);

    std::string getPluginName();
    IPlugin::Version getPluginVersion();
};

extern "C" {
    IPlugin* create();
    void destroy(IPlugin* plugin);
}