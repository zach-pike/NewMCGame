#include "billboardManager.hpp"
#include "shader/shader.hpp"
#include "utils/path.hpp"

BillboardManager::BillboardManager() {
    billboardShader = loadShaders(
        getResourcePath("shaders/billboardVertex.glsl"),
        getResourcePath("shaders/billboardFragment.glsl")
    );

    uniforms.cameraPosition = glGetUniformLocation(billboardShader, "cameraPosition");
    uniforms.cameraRightWorldspace = glGetUniformLocation(billboardShader, "cameraRightWorldspace");
    uniforms.cameraUpWorldspace = glGetUniformLocation(billboardShader, "cameraUpWorldspace");
    uniforms.cameraViewProjection = glGetUniformLocation(billboardShader, "cameraViewProjection");

    uniforms.modelPosition = glGetUniformLocation(billboardShader, "modelPosition");
}
    

BillboardManager::~BillboardManager() {
    glDeleteShader(billboardShader);
}

std::size_t BillboardManager::addBillboard(glm::vec3 position, std::string text) {
    auto billboard = std::make_unique<Billboard>(position, text);
    billboards.push_back(std::move(billboard));
}

BillboardManager::Uniforms BillboardManager::getBillboardUniforms() const {
    return uniforms;
}

const BillboardManager::BillboardList& BillboardManager::getBillboards() const {
    return billboards;
}
GLuint BillboardManager::getBillboardShader() const {
    return billboardShader;
}