#include "ChunkBorderDebugger.hpp"
#include "../world.hpp"

ChunkBorderDebugger::ChunkBorderDebugger() {
    glGenBuffers(1, &info.vertexBuffer);
    glGenBuffers(1, &info.colorBuffer);
}

void ChunkBorderDebugger::draw(const World& world) {
    std::vector<Vertex> vtx;
    std::vector<glm::vec3> colors;

    const auto chunkBorderColors = glm::vec3(1, 1, 0);
    for (int y=0; y<world.chunkSizeY(); y++) {
        for (int z=0; z<world.chunkSizeZ(); z++) {
            for(int x=0; x<world.chunkSizeX(); x++) {
                // Bottom +X line
                vtx.push_back(Vertex(x*16, y*16, z*16));
                vtx.push_back(Vertex((x + 1)*16, y*16, z*16));

                // Bottom +Z line
                vtx.push_back(Vertex(x*16, y*16, z*16));
                vtx.push_back(Vertex(x*16, y*16, (z + 1)*16));

                vtx.push_back(Vertex(x*16, y*16, (z + 1)*16));
                vtx.push_back(Vertex((x + 1)*16, y*16, (z + 1)*16));

                vtx.push_back(Vertex((x + 1)*16, y*16, z*16));
                vtx.push_back(Vertex((x + 1)*16, y*16, (z + 1)*16));

                // Vertical lines
                vtx.push_back(Vertex(x*16, y*16, z*16));
                vtx.push_back(Vertex(x*16, (y + 1)*16, z*16));

                vtx.push_back(Vertex(x*16, y*16, (z + 1)*16));
                vtx.push_back(Vertex(x*16, (y + 1)*16, (z + 1)*16));

                vtx.push_back(Vertex((x + 1)*16, y*16, z*16));
                vtx.push_back(Vertex((x + 1)*16, (y + 1)*16, z*16));

                vtx.push_back(Vertex((x + 1)*16, y*16, (z + 1)*16));
                vtx.push_back(Vertex((x + 1)*16, (y + 1)*16, (z + 1)*16));

                vtx.push_back(Vertex(x*16, (y + 1)*16, z*16));
                vtx.push_back(Vertex((x + 1)*16, (y + 1)*16, z*16));

                vtx.push_back(Vertex(x*16, (y + 1)*16, z*16));
                vtx.push_back(Vertex(x*16, (y + 1)*16, (z + 1)*16));

                vtx.push_back(Vertex(x*16, (y + 1)*16, (z + 1)*16));
                vtx.push_back(Vertex((x + 1)*16, (y + 1)*16, (z + 1)*16));

                vtx.push_back(Vertex((x + 1)*16, (y + 1)*16, z*16));
                vtx.push_back(Vertex((x + 1)*16, (y + 1)*16, (z + 1)*16));

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);

                colors.push_back(chunkBorderColors);
                colors.push_back(chunkBorderColors);
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, info.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(Vertex), vtx.data(), GL_STATIC_DRAW);

    info.nVerts = vtx.size();

    glBindBuffer(GL_ARRAY_BUFFER, info.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
}

ChunkBorderDebugger::ChunkBorderDebuggerDrawInfo ChunkBorderDebugger::getDrawingInfo() const {
    return info;
}

ChunkBorderDebugger::~ChunkBorderDebugger() {
    glDeleteBuffers(1, &info.vertexBuffer);
    glDeleteBuffers(1, &info.colorBuffer);
}