#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>

#include "hudTextItem/hudTextItem.hpp"

class HUDTextManager {
private:
    std::vector<std::shared_ptr<HUDTextItem>> textBoxes;

    GLuint shader;
    GLuint charTextureAtlas;

    GLuint textureUniform, scaleUniform, aspectUniform;

    bool gfxReady = false;
public:
    HUDTextManager();
    ~HUDTextManager();

    void gfxInit();

    std::shared_ptr<HUDTextItem> addTextItem(glm::vec2 pos, std::string text = "");

    void draw(float aspect);
};