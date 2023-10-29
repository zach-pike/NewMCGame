#pragma once

#include "ModManager/include/IMod.hpp"
#include "game.hpp"
#include "WorldGenWorker/WorldGenWorker.hpp"

#include <memory>

static const char* opts[] = { "Perlin(Normal)", "Cos" };

class NewWorldGen : public IMod {
private:
    bool windowOpen = true;

    int currentSelectedGenerator = 0;

    int newChunkSx = 25;
    int newChunkSy = 6;
    int newChunkSz = 25;
    float yScale = 80;

    // Settings for Perlin Noise Generator
    float xStretch = 0.01;
    float zStretch = 0.01;

    // Settings for Cos world generator
    float horizStretch = .1f;

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