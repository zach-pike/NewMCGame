#include "ExtraDebug.hpp"

#include <sstream>

IPlugin* create() {
    return new ExtraDebug();
}

void destroy(IPlugin* plugin) {
    delete plugin;
} 

void ExtraDebug::setup(Game& game) {
    debugText = game.getHUDTextManager().addTextItem(glm::vec2(-1, .4), "<test>");
    debugText->setScale(.2);
}

void ExtraDebug::frameUpdate(Game& game) {
    auto& playerPos = game.getPlayer().getPositionRef();

    std::stringstream ss;
    ss << "Pos: (";
    ss << playerPos.x;
    ss << ',';
    ss << playerPos.y;
    ss << ',';
    ss << playerPos.z;
    ss << ')';

    debugText->setText(ss.str());
}

void ExtraDebug::cleanup(Game& game) {

}

std::string ExtraDebug::getPluginName() {
    return "Extra Debug";
}

IPlugin::Version ExtraDebug::getPluginVersion() {
    return Version{ 1, 0 };
}