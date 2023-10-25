#include "world.hpp"

#include <GL/glew.h>

#include "glHelpers/texture/texture.hpp"
#include "glHelpers/shader/shader.hpp"
#include "glHelpers/utils/getEnvVar/getEnvVar.hpp"

#include <filesystem>
#include <string>
#include <fstream>
namespace fs = std::filesystem;

World::ChunkPos getChunkCoords(glm::vec3 pos) {
    int chunkX = std::floor(pos.x / 16.f);
    int chunkY = std::floor(pos.y / 16.f);
    int chunkZ = std::floor(pos.z / 16.f);

    return World::ChunkPos(chunkX, chunkY, chunkZ);
}

Chunk& World::getChunkFromWorldCoords(glm::vec3 pos) {
    return chunks.at(getChunkCoords(pos));
}

World::World() {}

World::~World() {
    if (gfxReady) {
        glDeleteProgram(worldShader);
    }
}

// Does all the necessary calls to OpenGL related functions
void World::gfxInit() {
    worldShader = loadShaders("world/");

    viewProjectionID = glGetUniformLocation(worldShader, "viewProjection");
    chunkCoordID = glGetUniformLocation(worldShader, "chunkCoord");
    textureAtlasID = glGetUniformLocation(worldShader, "textureAtlas");

    blockDB.gfxInit();
    blockDB.loadBlocks();

    gfxReady = true;
}

void World::generateEmptyMap(int sx, int sy, int sz) {
    chunks.clear();
    
    sizeX = sx;
    sizeY = sy;
    sizeZ = sz;

    for (int y=0; y<sy; y++) {
        for (int z=0; z<sz; z++) {
            for (int x=0; x<sx; x++) {
                auto pos = ChunkPos(x, y, z);
                chunks.emplace(pos, std::move(Chunk()));
            }
        }
    }
}

int World::chunkSizeX() const { return sizeX; }
int World::chunkSizeY() const { return sizeY; }
int World::chunkSizeZ() const { return sizeZ; }

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

    using namespace std;

    // If we set a block on the edge of a chunk we need to mark the neighbor to redraw
    if (!noCheck) {
        if (localX == 0 && get<0>(chunkCoords) > 0) {
            chunks.at(ChunkPos(get<0>(chunkCoords) - 1, get<1>(chunkCoords), get<2>(chunkCoords))).markRerender();
        }
        if (localX == 15 && get<0>(chunkCoords) < chunkSizeX() - 1) {
            chunks.at(ChunkPos(get<0>(chunkCoords) + 1, get<1>(chunkCoords), get<2>(chunkCoords))).markRerender();
        }

        if (localZ == 0 && get<2>(chunkCoords) > 0) {
            chunks.at(ChunkPos(get<0>(chunkCoords), get<1>(chunkCoords), get<2>(chunkCoords) - 1)).markRerender();
        }
        if (localZ == 15 && get<2>(chunkCoords) < chunkSizeZ() - 1) {
            chunks.at(ChunkPos(get<0>(chunkCoords), get<1>(chunkCoords), get<2>(chunkCoords) + 1)).markRerender();
        }

        if (localY == 0 && get<1>(chunkCoords) > 0) {
            chunks.at(ChunkPos(get<0>(chunkCoords), get<1>(chunkCoords) - 1, get<2>(chunkCoords))).markRerender();
        }
        if (localY == 15 && get<1>(chunkCoords) < chunkSizeY() - 1) {
            chunks.at(ChunkPos(get<0>(chunkCoords), get<1>(chunkCoords) + 1, get<2>(chunkCoords))).markRerender();
        }
    }

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

void World::update(std::size_t maxChunksToDraw) {
    std::size_t genChunks = 0;
    for (auto& a : chunks) {
        auto& chunk = a.second;
        if ((chunk.pendingMeshUpdate() != true) || (genChunks >= maxChunksToDraw)) continue;

        auto chunkPos = a.first;
        auto chunkCoord = glm::vec3(std::get<0>(chunkPos), std::get<1>(chunkPos), std::get<2>(chunkPos));

        chunk.generateMesh(chunkCoord, *this);
        genChunks++;
    }
}

void World::draw(const glm::mat4& viewProjection, glm::vec3 observerPosition, float maxViewDist) {
    lastNVerts = 0;

    // Render the world 
    glUseProgram(worldShader);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blockDB.getTextureId());
    glUniform1i(textureAtlasID, 0);
    
    glUniformMatrix4fv(viewProjectionID, 1, GL_FALSE, &viewProjection[0][0]);

    for (const auto& kv : getChunksRef()) {
        // Get all necessary data
        auto& chunk = kv.second;
        auto bufferInfo = chunk.getBufferInfo();
        auto chunkCoord = chunkPosToVec3(kv.first);

        auto dist = glm::length((chunkCoord*glm::vec3(16.f)) - observerPosition);
        if (dist > maxViewDist) continue;

        // Set the position origin of the chunk we are about to draw
        glUniform3fv(chunkCoordID, 1, &chunkCoord[0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.vertexBuffer);
        glVertexAttribPointer(
            0,                  // attribute 0.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.layerBuffer);
        glVertexAttribIPointer(
            1,                  // attribute 1.
            1,                  // size
            GL_INT,             // type
            0,                  // stride
            (void*)0            // array buffer offset
        );

        std::size_t n = chunk.getNVertices();
        // Draw the mesh
        glDrawArrays(GL_TRIANGLES, 0, n);

        lastNVerts += n;

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
}

std::size_t World::getLastNVerts() const {
    return lastNVerts;
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

BlockDB& World::getBlockDBRef() {
    return blockDB;
}

void World::moveChunks(World& w) {
    chunks = std::move(w.chunks);
    sizeX = w.sizeX;
    sizeY = w.sizeY;
    sizeZ = w.sizeZ;
}