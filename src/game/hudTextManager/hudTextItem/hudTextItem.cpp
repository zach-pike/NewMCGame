#include "hudTextItem.hpp"

#include <vector>
#include <array>
#include "types/types.hpp"

void HUDTextItem::drawMesh() {
    std::vector<glm::vec2> vertexes;
    std::vector<UV> uv;

    const float charDim = .2f * scale;

    const float textWidth = charDim * text.size();

    for (int i=0; i < text.size(); i++) {
        unsigned char letter = text.at(i);

        float xPosition = i * charDim;

        UV charUV = glm::vec2(letter% 16 , letter / 16);

        std::array<glm::vec2, 6> vtxs = {
            screenPosition + glm::vec2(xPosition, 0),                      // Bottom left
            screenPosition + glm::vec2(xPosition + charDim, charDim),      // Top right
            screenPosition + glm::vec2(xPosition, charDim),                // Top left
            screenPosition + glm::vec2(xPosition, 0),                      // Bottom left
            screenPosition + glm::vec2(xPosition + charDim, 0),            // Bottom right
            screenPosition + glm::vec2(xPosition + charDim, charDim),      // Top right
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

HUDTextItem::HUDTextItem(glm::vec2 _screenPosition, std::string _text, float _scale):
    screenPosition(_screenPosition),
    text(_text),
    scale(_scale)
{
    glGenBuffers(1, &buffers.vertexBuffer);
    glGenBuffers(1, &buffers.uvBuffer);

    drawMesh();
}

HUDTextItem::~HUDTextItem() {
    glDeleteBuffers(1, &buffers.vertexBuffer);
    glDeleteBuffers(1, &buffers.uvBuffer);
}

void HUDTextItem::setText(std::string _text) {
    text = _text;
    drawMesh();
}

std::string HUDTextItem::getText() const {
    return text;
}

glm::vec2 HUDTextItem::getScreenPosition() const {
    return screenPosition;
}

float HUDTextItem::getScale() const {
    return scale;
}

void HUDTextItem::setScreenPosition(glm::vec2 _screenPosition) {
    screenPosition = _screenPosition;
    drawMesh();
}

void HUDTextItem::setScale(float _scale) {
    scale = _scale;
    drawMesh();
}

HUDTextItem::Buffers HUDTextItem::getBuffers() const {
    return buffers;
}

std::size_t HUDTextItem::getNVerts() const {
    return nVerts;
}