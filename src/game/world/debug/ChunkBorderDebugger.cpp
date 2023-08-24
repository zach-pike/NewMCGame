#include "ChunkBorderDebugger.hpp"
#include "../world.hpp"

#include "shader/shader.hpp"
#include "utils/path.hpp"

ChunkBorderDebugger::ChunkBorderDebugger(const World& world) {
    glGenBuffers(1, &buffers.vertexBuffer);
    glGenBuffers(1, &buffers.colorBuffer);

    debugShader = loadShaders(
        getResourcePath("shaders/debug/vertex.glsl"),
        getResourcePath("shaders/debug/fragment.glsl")
    );

    viewProjectionID = glGetUniformLocation(debugShader, "viewProjection");

    drawLines(world.chunkSizeX(), world.chunkSizeY(), world.chunkSizeZ());
}

ChunkBorderDebugger::~ChunkBorderDebugger() {
    glDeleteBuffers(1, &buffers.vertexBuffer);
    glDeleteBuffers(1, &buffers.colorBuffer);

    glDeleteShader(debugShader);
}

void ChunkBorderDebugger::drawLines(int worldX, int worldY, int worldZ) {
    std::vector<Vertex> vtx;
    std::vector<glm::vec3> colors;

    const auto chunkBorderColors = glm::vec3(1, 1, 0);
    for (int y=0; y<worldX; y++) {
        for (int z=0; z<worldZ; z++) {
            for(int x=0; x<worldX; x++) {
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

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(Vertex), vtx.data(), GL_STATIC_DRAW);

    nVerts = vtx.size();

    glBindBuffer(GL_ARRAY_BUFFER, buffers.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
}

void ChunkBorderDebugger::draw(const glm::mat4& viewProjection) {
    glUseProgram(debugShader);
    glUniformMatrix4fv(viewProjectionID, 1, GL_FALSE, &viewProjection[0][0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.colorBuffer);
    glVertexAttribPointer(
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glDrawArrays(GL_LINES, 0, nVerts);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}