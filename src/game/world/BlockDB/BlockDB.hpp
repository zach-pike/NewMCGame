#pragma once

#include <GL/gl.h>

#include <map>
#include <vector>

#include "glHelpers/utils/logger/logger.hpp"

class BlockDB {
public:
    struct FaceLayers {
        unsigned int top, bottom, north, south, east, west;
    };

    struct BlockInfo {
        FaceLayers faces;
        int blockId;
    };    
private:
    GLuint textureArrayId;
    bool texturesLoaded = false;
    bool gfxReady = false;

    Logger logger{ "BlockDB", Logger::FGColors::BLUE };

    std::map<std::string, BlockInfo> blocks;
    std::vector<BlockInfo> blockById;
public:
    BlockDB();
    ~BlockDB();

    /**
     * Initiate any OpenGL components
    */
    void gfxInit();

    /**
     * Load all the files from BLOCKPACKS_FOLDER and load the texture defined in those files
    */
    void loadBlocks();

    /**
     * Get the ID of the created texture array
    */
    GLuint getTextureId();

    /**
     * Returns the block id for a block by name
     * @param name Name of the block
    */
    int getIdByName(std::string name) const;

    /**
     * Get the BlockInfo struct for a block of a certain id
     * @param idx Block ID to look up
    */
    const BlockInfo& getBlockInfoByID(int idx) const;

    /**
     * Get a const reference to the std::map of blocks
    */
    const std::map<std::string, BlockInfo>& getBlockMap() const;
};