#pragma once

#include "ModManager/include/IMod.hpp"
#include "billboardManager/billboard/billboard.hpp"
#include "game.hpp"

#include <memory>

class WorldInfoSign : public IMod {
private:
    std::shared_ptr<Billboard> billboard;
public:
    void setup(Game&);
    void frameUpdate(Game&);
    void cleanup(Game&);
    std::string getModName();
    Version getModVersion();
};

extern "C" {
    WorldInfoSign* create();
    void destroy(WorldInfoSign* plugin);
}