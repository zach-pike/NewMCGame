#pragma once

#include <GL/gl.h>

#include <map>

#include "glHelpers/utils/logger/logger.hpp"

class BlockDB {
public:
    struct FaceLayers {
        unsigned int top, bottom, north, south, east, west;
    };

    struct BlockInfo {
        FaceLayers faces;
    };

    Logger logger{ "BlockDB", Logger::FGColors::CYAN };
    
private:
    GLuint textureArrayId;
    bool texturesLoaded = false;
    bool gfxReady = false;

    std::map<std::string, BlockInfo> blocks;
public:
    BlockDB();
    ~BlockDB();

    // Called when opengl glfw context has been created
    void gfxInit();

    // Loads the TOML files from
    void loadBlocks();

    // Do not modify contents only use for binding to texture unit
    GLuint getTextureId();
};