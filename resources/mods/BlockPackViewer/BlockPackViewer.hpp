#pragma once

#include "ModManager/include/IMod.hpp"
#include "game.hpp"

class BlockPackViewer : public IMod {
private:

public:
    void setup(Game&);
    void frameUpdate(Game&);
    void cleanup(Game&);
    std::string getModName();
    Version getModVersion();
};

extern "C" {
    BlockPackViewer* create();
    void destroy(BlockPackViewer* plugin);
}