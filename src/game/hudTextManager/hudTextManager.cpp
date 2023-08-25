#include "hudTextManager.hpp"

#include "shader/shader.hpp"
#include "texture/texture.hpp"
#include "utils/path.hpp"

HUDTextManager::HUDTextManager() {}
HUDTextManager::~HUDTextManager() {
    if (gfxReady) {
        glDeleteProgram(shader);
        glDeleteTextures(1, &charTextureAtlas);
    }
}

void HUDTextManager::gfxInit() {
    shader = loadShaders(
        getResourcePath("shaders/hudText/vertex.glsl"),
        getResourcePath("shaders/hudText/fragment.glsl")
    );

    charTextureAtlas = loadImageTexture(getResourcePath("textures/TextAtlas.png"));
    textureUniform = glGetUniformLocation(shader, "myTexture");
    scaleUniform = glGetUniformLocation(shader, "scale");
    aspectUniform = glGetUniformLocation(shader, "aspect");

    gfxReady = true;
}

std::shared_ptr<HUDTextItem> HUDTextManager::addTextItem(glm::vec2 pos, std::string text) {
    auto textItem = std::make_shared<HUDTextItem>(pos, text);
    textBoxes.push_back(textItem);
    return textItem;
}

void HUDTextManager::draw(float aspect) {
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, charTextureAtlas);
    glUniform1i(textureUniform, 2);

    for (const auto& text : textBoxes) {
        auto buffers = text->getBuffers();
        auto scale = text->getScale();

        glUniform1f(aspectUniform, aspect);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, buffers.uvBuffer);
        glVertexAttribPointer(
            1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, text->getNVerts());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
}