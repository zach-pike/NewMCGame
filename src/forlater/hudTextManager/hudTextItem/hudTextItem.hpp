#pragma once

#include <glm/glm.hpp>
#include <string>

#include <GL/glew.h>

class HUDTextItem {
public:
    struct Buffers {
        GLuint vertexBuffer, uvBuffer;
    };
private:
    Buffers buffers;
    std::size_t nVerts;

    glm::vec2 screenPosition;
    float scale;
    std::string text;

    void drawMesh();
public:
    HUDTextItem(glm::vec2 screenPosition, std::string text, float scale = 1.f);
    ~HUDTextItem();

    void setText(std::string text);
    std::string getText() const;

    glm::vec2 getScreenPosition() const;
    float getScale() const;

    void setScreenPosition(glm::vec2 screenPosition);
    void setScale(float scaled);

    Buffers getBuffers() const;
    std::size_t getNVerts() const;
};