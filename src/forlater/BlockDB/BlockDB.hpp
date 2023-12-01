#pragma once

#include <GL/gl.h>

#include <map>
#include <vector>

#include "glHelpers/utils/logger/logger.hpp"

#define QuickNumMapImpl
#include "utils/QuickNumMap/QuickNumMap.hpp"

// TO BE UPDATED !!!!!!
/*
When you update this here are some things to think about:
this class should store the texture data for each pack and block
and also combine all the texture data into a single buffer to send over the net.
*/
class BlockDB {
public:
    struct FaceLayers {
        unsigned int top, bottom, north, south, east, west;
    };

    struct BlockInfo {
        FaceLayers faces;
        std::uint64_t packHash;
        int blockId;
    };

    using BlockIdent = std::pair<std::uint64_t, unsigned int>;
private:
    GLuint textureArrayId;
    bool texturesLoaded = false;
    bool gfxReady = false;

    Logger logger{ "BlockDB", Logger::FGColors::BLUE };

    std::map<std::string, BlockInfo> blocks;
    QuickNumMap<std::uint64_t, std::vector<BlockInfo>> blocksByHash;
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
     * Returns the block ident for a block by name
     * @param name Name of the block
    */
    BlockIdent getIdentByName(std::string name) const;

    /**
     * Get the BlockInfo struct for a block of a certain id
     * @param ident Block Ident to look up
    */
    const BlockInfo& lookupBlock(BlockIdent ident);

    /**
     * Get a const reference to the std::map of blocks
    */
    const std::map<std::string, BlockInfo>& getBlockMap() const;

    QuickNumMap<std::uint64_t, std::vector<BlockInfo>>& getQuickNumMapRef();
};

#undef QuickNumMapImpl