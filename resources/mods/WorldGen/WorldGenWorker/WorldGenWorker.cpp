#include "WorldGenWorker.hpp"

#include <functional>

void WorldGenWorker::_perlinWorldGenWorker(BlockDB* blockDBPtr, PerlinNoiseWorldGenDetails generatorSettings) {
    workerRunning = true;

    const auto& blockDB = *blockDBPtr;

    // Check for base block pack as this mod goes with it

    int dirtBlockID = blockDB.getIdByName("dirt");
    int grassBlockID = blockDB.getIdByName("grass");

	const siv::PerlinNoise perlin{ generatorSettings.seed };

    for (int z=0; z<generatorSettings.chunksZ*16; z++) {
        for (int x=0; x<generatorSettings.chunksX*16; x++) {
            const double yMax = perlin.octave2D_01((x * generatorSettings.xStretch), (z * generatorSettings.zStretch), 2) * generatorSettings.yScale;

            for(int y=0; y < yMax; y++) generatedWorld.setBlock(glm::vec3(x, y, z), Block(dirtBlockID), true);
            generatedWorld.setBlock(glm::vec3(x, yMax, z), Block(grassBlockID), true);
        }
    }

    worldIsReady = true;
    workerRunning = false;
}

void WorldGenWorker::_sineWorldGenWorker(BlockDB* blockDBPtr, SineWorldGenDetails generatorSettings) {
    workerRunning = true;

    const auto& blockDB = *blockDBPtr;

    // Check for base block pack as this mod goes with it

    int dirtBlockID = blockDB.getIdByName("dirt");
    int grassBlockID = blockDB.getIdByName("grass");
    for (int z=0; z<generatorSettings.chunksZ*16; z++) {
        for (int x=0; x<generatorSettings.chunksX*16; x++) {
            const double yMax = .25f * generatorSettings.scale * (std::cos(x * generatorSettings.horizStretch) + std::cos(z * generatorSettings.horizStretch) + 2.f);

            for(int y=0; y < yMax; y++) generatedWorld.setBlock(glm::vec3(x, y, z), Block(dirtBlockID), true);
            generatedWorld.setBlock(glm::vec3(x, yMax, z), Block(grassBlockID), true);
        }
    }

    worldIsReady = true;
    workerRunning = false;
}

WorldGenWorker::WorldGenWorker() {}
WorldGenWorker::~WorldGenWorker() {}


void WorldGenWorker::generatePerlinNoiseWorld(BlockDB& blockDB, PerlinNoiseWorldGenDetails generatorSettings) {
    if (workerRunning) return;
    worldIsReady = false;
    generatedWorld.generateEmptyMap(generatorSettings.chunksX, generatorSettings.chunksY, generatorSettings.chunksZ);
    worker = std::jthread(std::bind(&WorldGenWorker::_perlinWorldGenWorker, this, &blockDB, generatorSettings));
}

void WorldGenWorker::generateSineWaveWorld(BlockDB& blockDB, SineWorldGenDetails generatorSettings) {
    if (workerRunning) return;
    worldIsReady = false;
    generatedWorld.generateEmptyMap(generatorSettings.chunksX, generatorSettings.chunksY, generatorSettings.chunksZ);
    worker = std::jthread(std::bind(&WorldGenWorker::_sineWorldGenWorker, this, &blockDB, generatorSettings));
}

void WorldGenWorker::clearWorldReady() {
    worldIsReady = false;
}

World& WorldGenWorker::getGeneratedWorld() {
    return generatedWorld;
}