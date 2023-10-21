#pragma once

#include "ModManager/include/IMod.hpp"
#include "game.hpp"
#include "world/world.hpp"

#include <memory>

class NewWorldGen : public IMod {
private:
    bool windowOpen = true;

    int newChunkSx = 1;
    int newChunkSy = 1;
    int newChunkSz = 1;

    float xStretch = 0.01;
    float zStretch = 0.01;
    float yScale = 0;

    void worldGen(World&);
public:
    void setup(Game&);
    void frameUpdate(Game&);
    void cleanup(Game&);
    std::string getModName();
    Version getModVersion();
};

extern "C" {
    IMod* create();
    void destroy(IMod* plugin);
}