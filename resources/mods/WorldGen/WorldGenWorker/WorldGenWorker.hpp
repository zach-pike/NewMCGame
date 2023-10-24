#pragma once

#include <thread>
#include "PerlinNoise.hpp"
#include "world/world.hpp"

class WorldGenWorker {
public:
    struct WorldGenDetails {
        float xStretch, zStrecth;
        float yScale;

        int chunksX, chunksY, chunksZ;

        siv::PerlinNoise::seed_type seed;
    };
private:
    std::jthread worker;
    bool worldIsReady = false;
    World generatedWorld;

    void _worldGenWorkerFunc(BlockDB* blockDB, WorldGenDetails generatorSettings);
public:
    WorldGenWorker();
    ~WorldGenWorker();

    void startWorldGenerator(BlockDB& blockDB, WorldGenDetails generatorSettings);

    inline bool isWorldReady() const {
        return worldIsReady;
    }
    
    void clearWorldReady();
    World& getGeneratedWorld();
};