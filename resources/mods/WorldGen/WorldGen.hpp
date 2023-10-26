#pragma once

#include "ModManager/include/IMod.hpp"
#include "game.hpp"
#include "WorldGenWorker/WorldGenWorker.hpp"

#include <memory>

class NewWorldGen : public IMod {
private:
    bool windowOpen = true;

    int newChunkSx = 25;
    int newChunkSy = 6;
    int newChunkSz = 25;

    float xStretch = 0.01;
    float zStretch = 0.01;
    float yScale = 80;

    WorldGenWorker generatorWorker;
public:
    void setup(Game&);
    void frameUpdate(Game&);
    void cleanup(Game&);
    std::string getModName();
    Version getModVersion();
};

extern "C" {
    NewWorldGen* create();
    void destroy(NewWorldGen* plugin);
}