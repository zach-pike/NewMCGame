#pragma once

#include <map>
#include <tuple>
#include <vector>
#include <string>

#include "chunk/chunk.hpp"
#include "BlockDB/BlockDB.hpp"

class World {
public:
    using ChunkPos = std::tuple<int, int, int>;

    struct SerializedWorldData {
        int chunkSizeX, chunkSizeY, chunkSizeZ;
    };
private:
    std::map<ChunkPos, Chunk> chunks;
    int sizeX;
    int sizeY;
    int sizeZ;

    Chunk& getChunkFromWorldCoords(glm::vec3 pos);
    
    bool gfxReady = false;
    GLuint worldShader;
    GLuint viewProjectionID, chunkCoordID, textureAtlasID;

    BlockDB blockDB;

    std::size_t lastNVerts = 0;
public:
    World();
    ~World();

    // Does all the necessary calls to OpenGL related functions
    void gfxInit();
    
    int chunkSizeX() const;
    int chunkSizeY() const;
    int chunkSizeZ() const;

    /**
     * Get a block in the world
     * @param pos Block position
    */
    Block getBlock(glm::vec3 pos);

    /**
     * Set a block in the world
     * @param pos The position to set the block
     * @param block The block to place
     * @param noCheck Should skip the neighbor rerender
    */
    void setBlock(glm::vec3 pos, Block block, bool noCheck = false);

    /**
     * Returns true if the coordinates are in the world
     * @param pos Position to check
    */
    bool coordinatesInWorld(glm::vec3 pos);

    /**
     * Generate a world using Perlin noise
     * @param xs Number of chunks on the X Axis
     * @param ys Number of chunks on the Y Axis
     * @param zs Number of chunks on the Z Axis
    */
    void generateWorld(int xs, int ys, int zs);

    /**
     * Get a reference to a chunk at a chunkCoordinate
     * @param chunkCoords The coordinate of the chunk to get
    */
    Chunk& getChunkRef(glm::vec3 chunkCoords);

    /**
     * Get a reference to the container of Chunks
    */
    std::map<ChunkPos, Chunk>& getChunksRef();

    /**
     * Update all the chunks that need updating (should be called every frame)
    */
    void update();

    /**
     * Draw the chunk meshes for the world
     * @param viewProjection The camera View projection
    */
    void draw(const glm::mat4& viewProjection);

    /**
     * Returns the total number of vertices drawn at the last render
    */
    std::size_t getLastNVerts() const;

    /**
     * Save world to files
     * @param saveName The name to save the world under
    */
    void saveWorld(std::string saveName) const;

    /**
     * Load the world from a save
     * @param saveName The saveName to load
    */
    void loadWorld(std::string saveName);

    /**
     * Returns true if the saveName is a valid world save structure
    */
    bool worldSaveExists(std::string saveName) const;

    /**
     * Get a reference the BlockDB
    */
    BlockDB& getBlockDBRef();

    inline static glm::vec3 chunkPosToVec3(ChunkPos pos) {
        return glm::vec3(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
    }
};
