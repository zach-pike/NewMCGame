#include "billboardManager.hpp"
#include "shader/shader.hpp"
#include "texture/texture.hpp"
#include "utils/path.hpp"

BillboardManager::BillboardManager() {}

BillboardManager::~BillboardManager() {
    if (gfxReady) {
        glDeleteShader(billboardShader);
        glDeleteTextures(1, &billboardTexture);
    }
}

void BillboardManager::gfxInit() {
    billboardShader = loadShaders(
        getResourcePath("shaders/billboard/vertex.glsl"),
        getResourcePath("shaders/billboard/fragment.glsl")
    );

    billboardTexture = loadImageTexture(getResourcePath("textures/TextAtlas.png"));
    textureID = glGetUniformLocation(billboardShader, "myTexture");

    uniforms.cameraPosition = glGetUniformLocation(billboardShader, "cameraPosition");
    uniforms.cameraRightWorldspace = glGetUniformLocation(billboardShader, "cameraRightWorldspace");
    uniforms.cameraUpWorldspace = glGetUniformLocation(billboardShader, "cameraUpWorldspace");
    uniforms.cameraViewProjection = glGetUniformLocation(billboardShader, "cameraViewProjection");

    uniforms.modelPosition = glGetUniformLocation(billboardShader, "modelPosition");
    uniforms.billboardScale = glGetUniformLocation(billboardShader, "billboardScale");

    gfxReady = true;
}

std::size_t BillboardManager::addBillboard(glm::vec3 position, std::string text, std::string id) {
    auto billboard = std::make_shared<Billboard>(position, text, id);
    billboards.push_back(std::move(billboard));

    return billboards.size();
}

void BillboardManager::draw(glm::vec3 viewerPos, const glm::mat4& viewMatrix, const glm::mat4& viewProjection) {
    // Use the billboard rendering shader
    glUseProgram(billboardShader);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, billboardTexture);
    glUniform1i(textureID, 1);

    // Set some camera related variables
    glUniform3fv(uniforms.cameraPosition, 1, &viewerPos[0]);
    glUniformMatrix4fv(uniforms.cameraViewProjection, 1, GL_FALSE, &viewProjection[0][0]);

    glm::vec3 cameraRightWorldspace = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glm::vec3 cameraUpWorldspace = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    glUniform3fv(uniforms.cameraRightWorldspace, 1, &cameraRightWorldspace[0]);
    glUniform3fv(uniforms.cameraUpWorldspace, 1, &cameraUpWorldspace[0]);

    for (const auto& billboard : getBillboardsRef()) {
        auto position = billboard->getPosition();
        auto buffers = billboard->getDrawBuffers();
        auto scale = billboard->getScale();

        // Set the position of the billboard
        glUniform3fv(uniforms.modelPosition, 1, &position[0]); // Update this for every billboard
        glUniform2fv(uniforms.billboardScale, 1, &scale[0]);

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

        glDrawArrays(GL_TRIANGLES, 0, billboard->vertCount());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
}

std::shared_ptr<Billboard>& BillboardManager::getBillboardByID(std::string id) {
    for (auto& billboard : billboards) {
        if (billboard->getBillboardID() == id) return billboard;
    }

    throw std::runtime_error("No billboard with ID " + id);
}

BillboardManager::BillboardList& BillboardManager::getBillboardsRef() {
    return billboards;
}