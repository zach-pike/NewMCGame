#pragma once

#include <glm/glm.hpp>

#include <map>
#include <tuple>
#include <vector>
#include <string>

#include "chunk/chunk.hpp"

class World {
public:
    using ChunkPos = std::tuple<int, int, int>;

    struct SerializedWorldData {
        int chunkSizeX, chunkSizeY, chunkSizeZ;
    };
private:
    Chunk& getChunkFromWorldCoords(glm::vec3 pos);

    // Actual world container
    std::map<ChunkPos, Chunk> chunks;
    int sizeX;
    int sizeY;
    int sizeZ;
public:
    World();
    ~World();

    /**
     * Clears the world and initializes it with empty chunks
    */
    void generateEmptyMap(int sx, int sy, int sz);

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
     * Get a reference to a chunk at a chunkCoordinate
     * @param chunkCoords The coordinate of the chunk to get
    */
    Chunk& getChunkRef(glm::vec3 chunkCoords);

    /**
     * Get a reference to the container of Chunks
    */
    std::map<ChunkPos, Chunk>& getChunksRef();

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

    // Move chunks from another world object into this world
    void moveChunks(World& world);

    // World size getters
    int chunkSizeX() const;
    int chunkSizeY() const;
    int chunkSizeZ() const;

    inline static glm::vec3 chunkPosToVec3(ChunkPos pos) {
        return glm::vec3(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
    }
};
