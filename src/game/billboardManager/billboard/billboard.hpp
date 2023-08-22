#pragma once

#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>

class Billboard {
public:
    struct Buffers {
        GLuint vertexBuffer, uvBuffer;
    };
private:
    glm::vec3 position;
    std::string text;

    Buffers buffers;
    std::size_t nVerts;

    void draw();
public:
    Billboard(glm::vec3 position, std::string text);
    Billboard(const Billboard&) = delete;
    Billboard(const Billboard&&) = delete;

    glm::vec3 getPosition() const;
    Buffers getDrawBuffers() const;

    std::size_t vertCount() const;

    ~Billboard();
};