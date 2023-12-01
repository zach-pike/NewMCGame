#include "world.hpp"

#include "utils/getEnvVar/getEnvVar.hpp"

#include <filesystem>
#include <string>
#include <fstream>
#include <cmath>
namespace fs = std::filesystem;

static World::ChunkPos getChunkCoords(glm::vec3 pos) {
    int chunkX = std::floor(pos.x / 16.f);
    int chunkY = std::floor(pos.y / 16.f);
    int chunkZ = std::floor(pos.z / 16.f);

    return World::ChunkPos(chunkX, chunkY, chunkZ);
}

Chunk& World::getChunkFromWorldCoords(glm::vec3 pos) {
    return chunks.at(getChunkCoords(pos));
}

World::World() {}

World::~World() {}

void World::generateEmptyMap(int sx, int sy, int sz) {
    chunks.clear();
    
    sizeX = sx;
    sizeY = sy;
    sizeZ = sz;

    for (int y=0; y<sy; y++) {
        for (int z=0; z<sz; z++) {
            for (int x=0; x<sx; x++) {
                auto pos = ChunkPos(x, y, z);
                chunks.insert(std::make_pair(pos, Chunk{}));
            }
        }
    }
}

Block World::getBlock(glm::vec3 pos) {
    Chunk& chunk = getChunkFromWorldCoords(pos);

    float localX = std::floor(std::fmod(pos.x, 16.f));
    float localY = std::floor(std::fmod(pos.y, 16.f));
    float localZ = std::floor(std::fmod(pos.z, 16.f));

    return chunk.getBlock(glm::vec3(localX, localY, localZ));
}

void World::setBlock(glm::vec3 pos, Block b, bool noCheck) {
    ChunkPos chunkCoords = getChunkCoords(pos);
    Chunk& chunk = getChunkFromWorldCoords(pos);

    int localX = std::floor(std::fmod(pos.x, 16.f));
    int localY = std::floor(std::fmod(pos.y, 16.f));
    int localZ = std::floor(std::fmod(pos.z, 16.f));

    return chunk.setBlock(glm::vec3(localX, localY, localZ), b);
}

bool World::coordinatesInWorld(glm::vec3 pos) {
    float maxX = sizeX * 16;
    float maxY = sizeY * 16;
    float maxZ = sizeZ * 16;

    return pos.x > 0 && pos.x < maxX
        && pos.y > 0 && pos.y < maxY
        && pos.z > 0 && pos.z < maxZ;
}

Chunk& World::getChunkRef(glm::vec3 chunkCoord) {
    return chunks.at(ChunkPos(chunkCoord.x, chunkCoord.y, chunkCoord.z));
}

std::map<World::ChunkPos, Chunk>& World::getChunksRef() {
    return chunks;
}

void World::saveWorld(std::string saveName) const {
    fs::path basePath = fs::current_path();
    std::string saveFolderNameStr = getEnvironmentVar("SAVES_FOLDER");

    fs::path saveFolder = basePath / fs::path(saveFolderNameStr);

    if (!fs::is_directory(saveFolder)) fs::create_directories(saveFolder);
    
    // Check if the folder exists already
    fs::path worldDataFilepath = saveFolder / "world.bin";
    fs::path chunksFolder = saveFolder / "chunks";

    fs::create_directory(saveFolder);

    auto worldData = SerializedWorldData{
        .chunkSizeX = sizeX,
        .chunkSizeY = sizeY,
        .chunkSizeZ = sizeZ
    };

    std::fstream worldDataFile(worldDataFilepath, std::ios::out | std::ios::binary);
    worldDataFile.write((char*)&worldData, sizeof(worldData));
    worldDataFile.close();

    // Create the chunk data folder
    fs::create_directory(chunksFolder);

    // Write all the chunks
    for (const auto& chunk : chunks) {
        std::stringstream fnamess;
        fnamess << "/(";
        fnamess << std::get<0>(chunk.first);
        fnamess << ',';
        fnamess << std::get<1>(chunk.first);
        fnamess << ',';
        fnamess << std::get<2>(chunk.first);
        fnamess << ").chunk";

        fs::path fName = chunksFolder / fnamess.str();
        std::vector<std::uint8_t> chunkData = chunk.second.serialize();

        std::fstream chunkFile(fName, std::ios::out | std::ios::binary);
        chunkFile.write((char*)chunkData.data(), chunkData.size());
        chunkFile.close();
    }
}

void World::loadWorld(std::string saveName) {
    fs::path basePath = fs::current_path();
    std::string saveFolderNameStr = getEnvironmentVar("SAVES_FOLDER");

    fs::path saveFolder = basePath / fs::path(saveFolderNameStr);
    fs::path worldDataFilepath = saveFolder / "/world.bin";
    fs::path chunksFolder = saveFolder / "chunks";

    // Read the world data
    SerializedWorldData worldData = { 0, 0, 0 };
    std::fstream worldDataFile(worldDataFilepath, std::ios::in | std::ios::binary);
    worldDataFile.read((char*)&worldData, sizeof(SerializedWorldData));
    worldDataFile.close();

    // Clear the world
    chunks.clear();

    sizeX = worldData.chunkSizeX;
    sizeY = worldData.chunkSizeY;
    sizeZ = worldData.chunkSizeZ;

    // Load the chunks
    for (int y=0; y < worldData.chunkSizeY; y++) {
        for (int z=0; z < worldData.chunkSizeZ; z++) {
            for (int x=0; x < worldData.chunkSizeX; x++) {
                std::stringstream fnamess;
                fnamess << "/(";
                fnamess << x;
                fnamess << ',';
                fnamess << y;
                fnamess << ',';
                fnamess << z;
                fnamess << ").chunk";

                fs::path fName = chunksFolder / fnamess.str();
                std::fstream chunkFile(fName, std::ios::in | std::ios::binary);

                std::vector<std::uint8_t> chunkData;
                chunkData.resize(Chunk::serializedChunkSize());
                chunkFile.read((char*)chunkData.data(), chunkData.size());
                chunkFile.close();

                chunks[ChunkPos(x, y, z)].deserialize(chunkData);
            }
        }
    }
}

bool World::worldSaveExists(std::string saveName) const {
    fs::path basePath = fs::current_path();
    std::string saveFolderNameStr = getEnvironmentVar("SAVES_FOLDER");

    fs::path saveFolder = basePath / fs::path(saveFolderNameStr);
    std::string worldDataFilepath = saveFolder / "world.bin";
    std::string chunksFolder = saveFolder / "/chunks";

    return fs::is_directory(saveFolder) && fs::exists(worldDataFilepath) && fs::is_directory(chunksFolder);
}
void World::moveChunks(World& w) {
    chunks = std::move(w.chunks);
    sizeX = w.sizeX;
    sizeY = w.sizeY;
    sizeZ = w.sizeZ;
}

int World::chunkSizeX() const { return sizeX; }
int World::chunkSizeY() const { return sizeY; }
int World::chunkSizeZ() const { return sizeZ; }