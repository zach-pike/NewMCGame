#pragma once

#include <vector>
#include <memory>

#include "utils/NotCopyable.hpp"
#include "billboard/billboard.hpp"

class BillboardManager : private NonCopyable {
public:
    using BillboardList = std::vector<std::shared_ptr<Billboard>>;

    struct Uniforms {
        GLuint cameraPosition, cameraViewProjection, modelPosition, cameraRightWorldspace, cameraUpWorldspace, billboardScale;
    };
private:
    BillboardList billboards;

    bool gfxReady = false;
    GLuint billboardShader;
    GLuint billboardTexture;
    GLuint textureID;
    Uniforms uniforms;
public:
    BillboardManager();
    BillboardManager(const BillboardManager&&) = delete;
    ~BillboardManager();

    /**
     * Initialize everything thats dependent on OpenGL
    */
    void gfxInit();

    /**
     * Add a billboard into the world
    */
    std::size_t addBillboard(glm::vec3 position, std::string text, std::string id);

    /**
     * Draw all the billboards in the world
     * @param viewerPos The camera position in the world
     * @param viewMatrix The view matrix of the camera
     * @param viewProjection The viewProjection
    */
    void draw(glm::vec3 viewerPos, const glm::mat4& viewMatrix, const glm::mat4& viewProjection);

    /**
     * Get a shared pointer to a billboard by its ID
     * @param id The id of the billboard to look for
    */
    std::shared_ptr<Billboard>& getBillboardByID(std::string id);

    /**
     * Get a reference to the billboards list
    */
    BillboardList& getBillboardsRef();
};