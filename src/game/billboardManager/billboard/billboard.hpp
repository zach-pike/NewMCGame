#pragma once

#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>

class Billboard {
public:
    struct Buffers {
        GLuint vertexBuffer, uvBuffer;
    };
private:
    glm::vec3 position;
    glm::vec2 billboardScale = glm::vec2(1, 1);
    std::string text;
    std::string billboardID;
    

    Buffers buffers;
    std::size_t nVerts;

    void draw();
public:
    Billboard(glm::vec3 position, std::string text, std::string billboardID);
    Billboard(const Billboard&) = delete;
    Billboard(const Billboard&&) = delete;
    ~Billboard();
    
    /**
     * Get the position of the Billboard
    */
    glm::vec3 getPosition() const;

    /**
     * Set the position of the Billboard
    */
    void setPosition(glm::vec3);


    Buffers getDrawBuffers() const;

    /**
     * Gets the number of vertices in the buffer
    */
    std::size_t vertCount() const;

    /**
     * Set the text of the sign (redraws)
     * @param text text to put on the sign
    */
    void setText(std::string text);

    /**
     * Get the text of the Billboard
    */
    std::string getText() const;

    /**
     * Gets the billboards ID
    */
    const std::string& getBillboardID() const;

    /**
     * Gets the scale of the billboard
    */
    glm::vec2 getScale() const;

    /**
     * Set the scale of the billboard
    */
    void setScale(glm::vec2 scale);
};