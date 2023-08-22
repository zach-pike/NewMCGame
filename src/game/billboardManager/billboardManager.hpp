#pragma once

#include <vector>
#include <memory>

#include "utils/NotCopyable.hpp"
#include "billboard/billboard.hpp"

class BillboardManager : private NonCopyable {
public:
    using BillboardList = std::vector<std::unique_ptr<Billboard>>;

    struct Uniforms {
        GLuint cameraPosition, cameraViewProjection, modelPosition, cameraRightWorldspace, cameraUpWorldspace;
    };
private:
    BillboardList billboards;

    bool gfxReady = false;
    GLuint billboardShader;
    Uniforms uniforms;
public:
    BillboardManager();
    BillboardManager(const BillboardManager&&) = delete;
    ~BillboardManager();

    // Does all the necessary calls to OpenGL related functions
    void gfxInit();

    std::size_t addBillboard(glm::vec3 position, std::string text);

    Uniforms getBillboardUniforms() const;

    void draw(glm::vec3 viewerPos, const glm::mat4& viewMatrix, const glm::mat4& viewProjection);

    const BillboardList& getBillboards() const;
    GLuint getBillboardShader() const;
};