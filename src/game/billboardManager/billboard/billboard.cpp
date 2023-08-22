/*
class Billboard {
public:
    struct Buffers {
        GLuint vertexBuffer, uvBuffer;
    };
private:
    glm::vec3 position;
    std::string text;

        Buffers buffers;

    void draw();
public:
    Billboard(glm::vec3 position, std::string text);
    Billboard(const Billboard&) = delete;
    Billboard(const Billboard&&) = delete;

    glm::vec3 getPosition() const;
    Buffers getDrawBuffers() const;

    ~Billboard();
};
*/

#include "billboard.hpp"
#include "types/types.hpp"

#include <vector>

void Billboard::draw() {
    std::vector<Vertex> vertexes = {
        Vertex(0, 0, 0),
        Vertex(1, 0, 0),
        Vertex(1, 1, 0),
    };

    std::vector<UV> uv;

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex), vertexes.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(UV), uv.data(), GL_STATIC_DRAW);

    nVerts = vertexes.size();
}

Billboard::Billboard(glm::vec3 _position, std::string _text):
 position(_position),
 text(_text)
{
    glGenBuffers(1, &buffers.vertexBuffer);
    glGenBuffers(1, &buffers.uvBuffer);

    draw();
}

Billboard::~Billboard() {
    glDeleteBuffers(1, &buffers.vertexBuffer);
    glDeleteBuffers(1, &buffers.uvBuffer);
}

glm::vec3 Billboard::getPosition() const {
    return position;
}

Billboard::Buffers Billboard::getDrawBuffers() const {
    return buffers;
}

std::size_t Billboard::vertCount() const {
    return nVerts;
}