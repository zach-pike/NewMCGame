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
#include <array>
#include <glm/glm.hpp>
#include <iostream>

void Billboard::draw() {
    std::vector<glm::vec2> vertexes;
    std::vector<UV> uv;

    const float charDim = .2f;

    const float billboardWidth = charDim * text.size();
    const float charOffset = billboardWidth / 2.f;

    for (int i=0; i < text.size(); i++) {
        unsigned char letter = text.at(i);

        float xPosition = i * charDim - charOffset;

        UV charUV = glm::vec2(letter% 16 , letter / 16);

        std::array<glm::vec2, 6> vtxs = {
            glm::vec2(xPosition, 0),                      // Bottom left
            glm::vec2(xPosition + charDim, charDim),      // Top right
            glm::vec2(xPosition, charDim),                // Top left

            glm::vec2(xPosition, 0),                      // Bottom left
            glm::vec2(xPosition + charDim, 0),            // Bottom right
            glm::vec2(xPosition + charDim, charDim),      // Top right
        };

        std::array<UV, 6> uvs = {
            charUV + UV(0, 1),
            charUV + UV(1, 0),
            charUV,

            charUV + UV(0, 1),
            charUV + UV(1, 1),
            charUV + UV(1, 0),
        };

        vertexes.insert(vertexes.end(), vtxs.begin(), vtxs.end());
        uv.insert(uv.end(), uvs.begin(), uvs.end());
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(glm::vec2), vertexes.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(UV), uv.data(), GL_STATIC_DRAW);

    nVerts = vertexes.size();
}

Billboard::Billboard(glm::vec3 _position, std::string _text, std::string _billboardID):
 position(_position),
 text(_text),
 billboardID(_billboardID)
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

const std::string& Billboard::getBillboardID() const {
    return billboardID;
}

void Billboard::setText(std::string _text) {
    text = _text;
    draw();
}
std::string Billboard::getText() const {
    return text;
}