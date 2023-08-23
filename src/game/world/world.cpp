#include "world.hpp"

#include "../utils/PerlinNoise.hpp"
#include <GL/glew.h>

#include "texture/texture.hpp"
#include "shader/shader.hpp"
#include "utils/path.hpp"

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
        glDeleteTextures(1, &textureAtlas);
        glDeleteShader(worldShader);
    }
}

// Does all the necessary calls to OpenGL related functions
void World::gfxInit() {
    worldShader = loadShaders(
        getResourcePath("shaders/world/vertex.glsl"),
        getResourcePath("shaders/world/fragment.glsl")
    );

    viewProjectionID = glGetUniformLocation(worldShader, "viewProjection");
    chunkCoordID = glGetUniformLocation(worldShader, "chunkCoord");
    textureAtlasID = glGetUniformLocation(worldShader, "textureAtlas");

    textureAtlas = loadImageTexture(getResourcePath("textures/Chunk.bmp"));

    gfxReady = true;
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

void World::generateWorld(int xs, int ys, int zs) {
    chunks.clear();

    sizeX = xs;
    sizeY = ys;
    sizeZ = zs;

    for (int y=0; y<ys; y++) {
        for (int z=0; z<zs; z++) {
            for (int x=0; x<xs; x++) {
                auto pos = ChunkPos(x, y, z);

                chunks.emplace(pos, std::move(Chunk()));
            }
        }
    }

    const siv::PerlinNoise::seed_type seed = 1743993u;
	const siv::PerlinNoise perlin{ seed };

    for (int z=0; z<sizeZ*16; z++) {
        for (int x=0; x<sizeX*16; x++) {
            const double yMax = perlin.octave2D_01((x * 0.01), (z * 0.01), 4) * 50 + 10;

            for(int y=0; y < yMax; y++) setBlock(glm::vec3(x, y, z), Block(Block::BlockType::DIRT));
            setBlock(glm::vec3(x, yMax, z), Block(Block::BlockType::GRASS), true);
        }
    }
}

Chunk& World::getChunk(glm::vec3 chunkCoord) {
    return chunks.at(ChunkPos(chunkCoord.x, chunkCoord.y, chunkCoord.z));
}

std::map<World::ChunkPos, Chunk>& World::getChunks() {
    return chunks;
}

void World::update() {
    for (auto& a : chunks) {
        auto& chunk = a.second;
        if (!chunk.pendingMeshUpdate()) continue;

        auto chunkPos = a.first;
        auto chunkCoord = glm::vec3(std::get<0>(chunkPos), std::get<1>(chunkPos), std::get<2>(chunkPos));

        chunk.generateMesh(chunkCoord, *this);
    }
}

void World::draw(const glm::mat4& viewProjection) {
    // Render the world 
    glUseProgram(worldShader);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
    glUniform1i(textureAtlasID, 0);
    
    glUniformMatrix4fv(viewProjectionID, 1, GL_FALSE, &viewProjection[0][0]);

    for (const auto& kv : getChunks()) {
        // Get all necessary data
        auto& chunk = kv.second;
        auto bufferInfo = chunk.getBufferInfo();
        auto chunkCoord = chunkPosToVec3(kv.first);

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
        glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.uvBuffer);
        glVertexAttribPointer(
            1,                  // attribute 1.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        // Draw the mesh
        glDrawArrays(GL_TRIANGLES, 0, chunk.getNVertices()); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
}

std::size_t World::getNVertices() const {
    std::size_t n = 0;
    for (auto& chunk : chunks) {
        n += chunk.second.getNVertices();
    }

    return n;
}