#include "WorldGenWorker.hpp"

#include <functional>

/*
class WorldGenWorker {
public:
    struct WorldGenDetails {
        float xStretch, zStrecth;
        float yScale;

        int chunksX, chunksY, chunksZ;

        siv::PerlinNoise::seed_type seed;
    };
private:
    std::thread worker;
    bool worldIsReady = false;
    World generatedWorld;
public:
    WorldGenWorker();
    ~WorldGenWorker();

    void startWorldGenerator(WorldGenDetails generatorSettings);

    bool isWorldReady() const;
    World& getGeneratedWorld();
};
*/

void WorldGenWorker::_worldGenWorkerFunc(BlockDB* blockDBPtr, WorldGenDetails generatorSettings) {
    const auto& blockDB = *blockDBPtr;

    // Check for base block pack as this mod goes with it

    int dirtBlockID = blockDB.getIdByName("dirt");
    int grassBlockID = blockDB.getIdByName("grass");

	const siv::PerlinNoise perlin{ generatorSettings.seed };

    for (int z=0; z<generatorSettings.chunksZ*16; z++) {
        for (int x=0; x<generatorSettings.chunksX*16; x++) {
            const double yMax = perlin.octave2D_01((x * generatorSettings.xStretch), (z * generatorSettings.zStrecth), 2) * generatorSettings.yScale;

            for(int y=0; y < yMax; y++) generatedWorld.setBlock(glm::vec3(x, y, z), Block(dirtBlockID), true);
            generatedWorld.setBlock(glm::vec3(x, yMax, z), Block(grassBlockID), true);
        }
    }

    worldIsReady = true;
}

WorldGenWorker::WorldGenWorker() {}
WorldGenWorker::~WorldGenWorker() {}


void WorldGenWorker::startWorldGenerator(BlockDB& blockDB, WorldGenDetails generatorSettings) {
    worldIsReady = false;
    generatedWorld.generateEmptyMap(generatorSettings.chunksX, generatorSettings.chunksY, generatorSettings.chunksZ);
    worker = std::jthread(std::bind(&WorldGenWorker::_worldGenWorkerFunc, this, &blockDB, generatorSettings));
}

void WorldGenWorker::clearWorldReady() {
    worldIsReady = false;
}

World& WorldGenWorker::getGeneratedWorld() {
    return generatedWorld;
}