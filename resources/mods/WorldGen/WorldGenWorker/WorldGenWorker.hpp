#pragma once

#include <thread>
#include "PerlinNoise.hpp"
#include "world/world.hpp"

class WorldGenWorker {
public:
    struct PerlinNoiseWorldGenDetails {
        float xStretch, zStretch;
        float yScale;

        int chunksX, chunksY, chunksZ;

        siv::PerlinNoise::seed_type seed;
    };

    struct SineWorldGenDetails {
        int chunksX, chunksY, chunksZ;

        float scale, horizStretch;
    };
private:
    std::jthread worker;
    bool workerRunning = false;

    World generatedWorld;
    bool worldIsReady = false;

    void _perlinWorldGenWorker(BlockDB* blockDB, PerlinNoiseWorldGenDetails generatorSettings);
    void _sineWorldGenWorker(BlockDB* blockDB, SineWorldGenDetails generatorSettings);
public:
    WorldGenWorker();
    ~WorldGenWorker();

    void generatePerlinNoiseWorld(BlockDB& blockDB, PerlinNoiseWorldGenDetails generatorSettings);
    void generateSineWaveWorld(BlockDB& blockDB, SineWorldGenDetails generatorSettings);

    inline bool isWorldReady() const {
        return worldIsReady;
    }

    inline bool isWorkerRunning() const {
        return workerRunning;
    }
    
    void clearWorldReady();
    World& getGeneratedWorld();
};