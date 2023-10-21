#include <GL/glew.h>

#include "BlockDB.hpp"

#include "glHelpers/utils/getEnvVar/getEnvVar.hpp"
#include "tomlplusplus/toml.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "glHelpers/utils/stb_image.h"

#include <filesystem>
namespace fs = std::filesystem;

BlockDB::BlockDB() {}
BlockDB::~BlockDB() {
    if (gfxReady) {
        glDeleteTextures(1, &textureArrayId);
    }
}

void BlockDB::gfxInit() {
    // Create the texture & set filtering
    glGenTextures(1, &textureArrayId);

    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayId);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    gfxReady = true;
}

void BlockDB::loadBlocks() {
    blocks.clear();
    blockById.clear();

    // Get the path to all the block definitions
    auto blockPackFolder = fs::absolute(getEnvironmentVar("BLOCKPACKS_FOLDER"));
    auto blockPackFolderIter = fs::directory_iterator(blockPackFolder);

    // Base path of the textures folder
    auto texturesFolder = fs::absolute(getEnvironmentVar("TEXTURES_FOLDER"));

    // Array that holds the texture data of all the textures
    std::size_t nTextures = 0;
    std::vector<std::uint8_t> finalTextureArray;

    // Loop over all files in the BlockPacks folder
    for (auto file : blockPackFolderIter) {
        // Parse the TOML
        auto tomlData = toml::parse_file(file.path().c_str());

        // Information for the loader
        const auto& tomlLoaderInfo = tomlData["loaderinfo"];

        // Get pack name
        auto blockPackNameOpt = tomlLoaderInfo["name"].value<std::string>();
        if (!blockPackNameOpt.has_value()) {
            logger.warn("Failed to load block definition file " + std::string(file.path()) + ". Skipped.");
            continue;
        }
        std::string blockPackName = blockPackNameOpt.value();
        
        // Get the `filesToLoad` array
        auto& tomlImagesToLoad = *tomlLoaderInfo["filesToLoad"].as_array();

        // Folder containing all the images for the current pack were loading
        fs::path packFolder = texturesFolder / "world" / blockPackName;

        // ---------- START LOADING IMAGES ----------

        // This becomes true if we encounter a error loading in the for loop
        bool continueToNextPack = false;

        // Loaded texture pixel data
        std::vector<std::vector<std::uint8_t>> textures;

        // Load each image file
        for (auto& tomlImagePath : tomlImagesToLoad) {
            std::string imagePath = tomlImagePath.ref<std::string>();

            // Full path to the image file
            std::string fullImagePath = packFolder / imagePath;

            // Use stbi to load the image
            int textureWidth, textureHeight;
            std::uint8_t* imgRawData = stbi_load(fullImagePath.c_str(), &textureWidth, &textureHeight, nullptr, STBI_rgb_alpha);
            
            // Only supports 16x16 images right now
            if (textureWidth != 16 || textureHeight != 16) {
                logger.warn("Block Pack \"" + blockPackName + "\" image " + imagePath + " is NOT 16x16. Skipping.");

                continueToNextPack = true;
                break;
            }

            // Image size in bytes of the texture
            std::size_t imageByteSize = textureWidth * textureHeight * 4;

            // If we failed to open the image
            if (imgRawData == nullptr) {
                logger.warn("Failed to load image for BlockPack " + blockPackName);
                
                // If we couldn't load the image skip the pack
                continueToNextPack = true;
                break;
            }
            
            // Add image data to array
            textures.push_back(std::vector<std::uint8_t>(imgRawData, imgRawData + imageByteSize));

            // Free image
            stbi_image_free(imgRawData);
        }
        if (continueToNextPack) continue;

        // Load in the block face textures and add nTextures to each index
        auto& blocksTomlTable = *tomlData["blocks"].as_table();
        std::size_t nBlocks = 0;

        for (auto& item : blocksTomlTable) {
            std::string blockName = std::string(item.first.str());
            auto& faceTable = *(*item.second.as_table())["faces"].as_table();

            FaceLayers faces;
            faces.top = faceTable["top"].as_integer()->get() + nTextures;
            faces.bottom = faceTable["bottom"].as_integer()->get() + nTextures;
            faces.north = faceTable["north"].as_integer()->get() + nTextures;
            faces.south = faceTable["south"].as_integer()->get() + nTextures;
            faces.east = faceTable["east"].as_integer()->get() + nTextures;
            faces.west = faceTable["west"].as_integer()->get() + nTextures;
        
            int newBlockId = blocks.size() + 1;

            blocks[blockName] = BlockInfo{ .faces = faces, .blockId = newBlockId };
            blockById.push_back(BlockInfo{ .faces = faces, .blockId = newBlockId });
        }

        // Add number of textures from this pack to the total
        nTextures += textures.size();

        // Add all textures to main array
        for (const auto& textData : textures)
            finalTextureArray.insert(finalTextureArray.end(), textData.begin(), textData.end());

        // WE ARE DONE!!!!!
        logger.info("Loaded BlockPack \"" + blockPackName + "\"!");
    }

    for (const auto& p : blocks) {
        blockById.push_back(p.second);
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayId);

    // Allocate the storage && Upload pixel data.
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 16, 16, nTextures);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 16, 16, nTextures, GL_RGBA, GL_UNSIGNED_BYTE, finalTextureArray.data());
}

GLuint BlockDB::getTextureId() {
    return textureArrayId;
}

const BlockDB::BlockInfo& BlockDB::getBlockInfoByID(int idx) const {
    return blockById.at(idx - 1);
}

int BlockDB::getIdByName(std::string name) const {
    return blocks.at(name).blockId;
}

const std::map<std::string, BlockDB::BlockInfo>& BlockDB::getBlockMap() const {
    return blocks;
}