#pragma once

#include <thread>
#include "PerlinNoise.hpp"
#include "world/world.hpp"

class WorldGenWorker {
public:
    struct WorldGenDetails {
        float xStretch, zStretch;
        float yScale;

        int chunksX, chunksY, chunksZ;

        siv::PerlinNoise::seed_type seed;
    };
private:
    std::jthread worker;
    bool workerRunning = false;

    World generatedWorld;
    bool worldIsReady = false;

    void _worldGenWorkerFunc(BlockDB* blockDB, WorldGenDetails generatorSettings);
public:
    WorldGenWorker();
    ~WorldGenWorker();

    void startWorldGenerator(BlockDB& blockDB, WorldGenDetails generatorSettings);

    inline bool isWorldReady() const {
        return worldIsReady;
    }

    inline bool isWorkerRunning() const {
        return workerRunning;
    }
    
    void clearWorldReady();
    World& getGeneratedWorld();
};