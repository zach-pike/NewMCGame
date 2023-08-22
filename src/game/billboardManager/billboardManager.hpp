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

    GLuint billboardShader;

    Uniforms uniforms;
public:
    BillboardManager();
    BillboardManager(const BillboardManager&&) = delete;
    ~BillboardManager();

    std::size_t addBillboard(glm::vec3 position, std::string text);

    Uniforms getBillboardUniforms() const;

    const BillboardList& getBillboards() const;
    GLuint getBillboardShader() const;
};