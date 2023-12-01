# Chunk Mesh Renderer
```c++
void Chunk::buildMesh(World& world, glm::vec3 chunkCoords) {
    meshUpdatedNeeded = false;

    std::vector<glm::vec3> vertices;
    std::vector<GLint> layers;

    // Chunk generation code
    for (int y=0; y<16; y++) {
        for (int z=0; z<16; z++) {
            for (int x=0; x<16; x++) {
                auto cPos = glm::vec3(x, y, z);

                // Get the block by using its chunk coordinates
                auto block = getBlockReference(cPos);
                auto ident = block.getIdent();

                // if its air just do nothing
                if (ident.second == 0) continue;

                using namespace glm;

                auto addFace = [&](std::array<glm::vec3, 6> verts, Block::BlockFace face) {
                    vertices.insert(vertices.end(), verts.begin(), verts.end());
                    auto data = world.getBlockDBRef().lookupBlock(ident);

                    int layerId;
                    switch(face){
                        case Block::BlockFace::NORTH:  layerId = data.faces.north; break;
                        case Block::BlockFace::SOUTH:  layerId = data.faces.south; break;
                        case Block::BlockFace::EAST:   layerId = data.faces.east;  break;
                        case Block::BlockFace::WEST:   layerId = data.faces.west;  break;
                        case Block::BlockFace::TOP:    layerId = data.faces.top;   break;
                        case Block::BlockFace::BOTTOM: layerId = data.faces.bottom; break;
                    }

                    for (int i=0; i<6; i++) layers.push_back(layerId);
                };
                
                // +X Face Check
                const std::array<glm::vec3, 6> xPos = {
                    glm::vec3(x + 1, y, z), glm::vec3(x + 1, y + 1, z),     glm::vec3(x + 1, y + 1, z + 1),
                    glm::vec3(x + 1, y, z), glm::vec3(x + 1, y + 1, z + 1), glm::vec3(x + 1, y, z + 1),
                };
                if (x >= 15) {
                    if (chunkCoords.x + 1 >= world.chunkSizeX()
                    || world.getChunkRef(chunkCoords + vec3(1, 0, 0)).getBlock(vec3(0.f, cPos.y, cPos.z)).getBlockId() == 0)
                        addFace(xPos, Block::BlockFace::NORTH);
                } else if (getBlockReference(cPos + vec3(1, 0, 0)).getBlockId() == 0)
                    addFace(xPos, Block::BlockFace::NORTH);

                // -X Face Check
                const std::array<glm::vec3, 6> xNeg = {
                    glm::vec3(x, y, z + 1), glm::vec3(x, y + 1, z + 1), glm::vec3(x, y + 1, z),
                    glm::vec3(x, y, z + 1), glm::vec3(x, y + 1, z),     glm::vec3(x, y, z),
                };
                if (x < 1) {
                    if (chunkCoords.x - 1 < 0
                    || world.getChunkRef(chunkCoords + vec3(-1, 0, 0)).getBlock(vec3(15.f, cPos.y, cPos.z)).getBlockId() == 0)
                        addFace(xNeg, Block::BlockFace::SOUTH);
                } else if (getBlockReference(cPos + vec3(-1, 0, 0)).getBlockId() == 0)
                    addFace(xNeg, Block::BlockFace::SOUTH);

                // +Z Face Check
                const std::array<glm::vec3, 6> zPos = {
                    glm::vec3(x + 1, y, z + 1), glm::vec3(x + 1, y + 1, z + 1), glm::vec3(x, y + 1, z + 1),
                    glm::vec3(x + 1, y, z + 1), glm::vec3(x, y + 1, z + 1),     glm::vec3(x, y, z + 1),
                };
                if (z >= 15) {
                    if (chunkCoords.z + 1 >= world.chunkSizeZ()
                    || world.getChunkRef(chunkCoords + vec3(0, 0, 1)).getBlock(vec3(cPos.x, cPos.y, 0)).getBlockId() == 0)
                        addFace(zPos, Block::BlockFace::EAST);
                } else if (getBlockReference(cPos + vec3(0, 0, 1)).getBlockId() == 0)
                    addFace(zPos, Block::BlockFace::EAST);

                // -Z Face Check
                const std::array<glm::vec3, 6> zNeg = {
                    glm::vec3(x, y, z), glm::vec3(x, y + 1, z),     glm::vec3(x + 1, y + 1, z),
                    glm::vec3(x, y, z), glm::vec3(x + 1, y + 1, z), glm::vec3(x + 1, y, z),
                };
                if (z < 1) {
                    if (chunkCoords.z - 1 < 0
                    || world.getChunkRef(chunkCoords + vec3(0, 0, -1)).getBlock(vec3(cPos.x, cPos.y, 15)).getBlockId() == 0)
                        addFace(zNeg, Block::BlockFace::WEST);
                } else if (getBlockReference(cPos + vec3(0, 0, -1)).getBlockId() == 0)
                    addFace(zNeg, Block::BlockFace::WEST);

                // +Y Face Check
                const std::array<glm::vec3, 6> yPos = {
                    glm::vec3(x, y + 1, z), glm::vec3(x, y + 1, z + 1),     glm::vec3(x + 1, y + 1, z + 1),
                    glm::vec3(x, y + 1, z), glm::vec3(x + 1, y + 1, z + 1), glm::vec3(x + 1, y + 1, z),
                };
                if (y >= 15) {
                    if (chunkCoords.y + 1 >= world.chunkSizeY()
                    || world.getChunkRef(chunkCoords + vec3(0, 1, 0)).getBlock(vec3(cPos.x, 0, cPos.z)).getBlockId() == 0)
                        addFace(yPos, Block::BlockFace::TOP);
                } else if (getBlockReference(cPos + vec3(0, 1, 0)).getBlockId() == 0)
                    addFace(yPos, Block::BlockFace::TOP);

                // -Y Face Check
                const std::array<glm::vec3, 6> yNeg = {
                    glm::vec3(x, y, z), glm::vec3(x + 1, y, z),     glm::vec3(x + 1, y, z + 1),
                    glm::vec3(x, y, z), glm::vec3(x + 1, y, z + 1), glm::vec3(x, y, z + 1),
                };
                if (y < 1) {
                    if (chunkCoords.y - 1 < 0
                    || world.getChunkRef(chunkCoords + vec3(0, -1, 0)).getBlock(vec3(cPos.x, 15, cPos.z)).getBlockId() == 0)
                        addFace(yNeg, Block::BlockFace::BOTTOM);
                } else if (getBlockReference(cPos + vec3(0, -1, 0)).getBlockId() == 0)
                    addFace(yNeg, Block::BlockFace::BOTTOM);
            }
        }
    }
    
    mesh.bufferChunkData(vertices, layers);
}
```

# All mesh drawer
```c++
void World::drawMeshes(const glm::mat4& viewProjection, glm::vec3 observerPosition, float maxViewDist) {
    totalVertexCount = 0;

    // Render the world 
    glUseProgram(worldShader);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blockDB.getTextureId());
    glUniform1i(textureAtlasID, 0);
    
    glUniformMatrix4fv(viewProjectionID, 1, GL_FALSE, &viewProjection[0][0]);

    for (const auto& kv : getChunksRef()) {
        // Get all necessary data
        auto& chunk = kv.second;
        auto& chunkMesh = chunk.getMeshRef();
        auto chunkCoord = chunkPosToVec3(kv.first);

        auto dist = glm::length((chunkCoord*glm::vec3(16.f))+glm::vec3(8,8,8) - observerPosition);
        if (dist > maxViewDist) continue;

        // Set the position origin of the chunk we are about to draw
        glUniform3fv(chunkCoordID, 1, &chunkCoord[0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.getVertexBuffer());
        glVertexAttribPointer(
            0,                  // attribute 0.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, chunkMesh.getLayerBuffer());
        glVertexAttribIPointer(
            1,                  // attribute 1.
            1,                  // size
            GL_INT,             // type
            0,                  // stride
            (void*)0            // array buffer offset
        );

        std::size_t n = chunkMesh.getVertexCount();
        glDrawArrays(GL_TRIANGLES, 0, n);

        totalVertexCount += n;

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
}
```

# Player controls
```c++
const float mouseSens = .005f;
const float moveSpeed = .05f;

void Player::updatePlayer(Game& game, float deltaTime) {
    static double pitch = 2.8f;
    static double yaw = 0;
    static double mx, my = 0.f;
    const static auto dirtId = game.getWorldRef().getBlockDBRef().getIdentByName("dirt");

    // Half screen width and heigh
    const int hwidth = game.getWindowWidth() / 2;
    const int hheight = game.getWindowHeight() / 2;

    if (settings.mouseLookFocus) {
        // Get screen width and height

        // First, do the player look code

        glfwGetCursorPos(game.getGLFWwindow(), &mx, &my);

        // Subtract the halfWidth and halfHeight to get a mouse movement amount based around (0, 0)
        mx -= hwidth;
        my -= hheight;

        // Set the cursor back to the middle of the frame
        glfwSetCursorPos(game.getGLFWwindow(), hwidth, hheight);
        
        // Yaw is the left to right movement of the camera
        // mouseSens is a scalar to control move speed
        yaw += mx * mouseSens;

        // Lock the camera to almost all the way down and almost all the way up
        pitch = std::min(1.5f*M_PI - .01, std::max((double)M_PI_2 + .01, pitch + my * mouseSens));

        // Calculate the look vector
        looking = glm::vec3(cos(pitch)*cos(yaw), sin(pitch), sin(yaw) * cos(pitch));
    }

    // Now player movement
    // Forward
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
        position += looking * moveSpeed * deltaTime;
    }
    // Back
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) {
        position -= looking * moveSpeed * deltaTime;
    }
    // Left
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS) {
        position += glm::vec3(looking.z, 0, -looking.x) * moveSpeed * deltaTime;
    }
    // Right
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS) {
        position -= glm::vec3(looking.z, 0, -looking.x) * moveSpeed * deltaTime;
    }
    // Up
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += glm::vec3(0, 1, 0) * moveSpeed * deltaTime;
    }
    // Down
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        position -= glm::vec3(0, 1, 0) * moveSpeed * deltaTime;
    }

    // Show wireframe toggle
    static bool wireframeButtonWasPressed = false;
    static bool wireframeEnabled = false;
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_R) == GLFW_PRESS) {
        if (wireframeButtonWasPressed == false) {
            wireframeButtonWasPressed = true;
            
            // Toggle wireframe
            wireframeEnabled = !wireframeEnabled;
            if (wireframeEnabled) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    } else {
        wireframeButtonWasPressed = false;
    }

    // Break block controller
    static bool breakButtonWasPressed = false;
    if (glfwGetMouseButton(game.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (breakButtonWasPressed == false) {
            breakButtonWasPressed = true;
            
            // Cast a ray and try to break the block we are looking at
            Ray r(position, looking);
            r.tryBreakBlock(game.getWorldRef(), 30);
        }
    } else {
        breakButtonWasPressed = false;
    }

    // Place block controller
    static bool placeButtonWasPressed = false;
    if (glfwGetMouseButton(game.getGLFWwindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (placeButtonWasPressed == false) {
            placeButtonWasPressed = true;
            
            // Cast a ray and try to break the block we are looking at
            Ray r(position, looking);
            r.tryPlaceBlock(game.getWorldRef(), Block(dirtId));
        }
    } else {
        placeButtonWasPressed = false;
    }

    static bool debugButtonWasPressed = false;
    if(glfwGetKey(game.getGLFWwindow(), GLFW_KEY_T) == GLFW_PRESS) {
        if (debugButtonWasPressed == false) {
            debugButtonWasPressed = true;

            settings.showDebug = !settings.showDebug;
        }
    } else {
        debugButtonWasPressed = false;
    }

    static bool mouseUnlockWasPressed = false;
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_F) == GLFW_PRESS) {
        if (mouseUnlockWasPressed == false) {
            mouseUnlockWasPressed = true;

            if (settings.mouseLookFocus)
                glfwSetInputMode(game.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else {
                glfwSetInputMode(game.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetCursorPos(game.getGLFWwindow(), hwidth, hheight);
            }

            settings.mouseLookFocus = !settings.mouseLookFocus;
        }
    } else {
        mouseUnlockWasPressed = false;
    }

    static bool createSavePressed = false;
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_H) == GLFW_PRESS) {
        if (createSavePressed == false) {
            createSavePressed = true;

            game.getWorldRef().saveWorld("test");
        }
    } else {
        createSavePressed = false;
    }
}
```

# Camera code
```c++
glm::mat4 Player::getCameraProjectionMatrix(float aspect) const {
    return glm::perspective(glm::radians(settings.fov), aspect, 0.01f, 1000.0f);
}

glm::mat4 Player::getCameraViewMatrix() const {
    return glm::lookAt(
		position, // Camera is at (4,3,3), in World Space
		position + looking, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 Player::getViewProjection(float aspect) const {
    auto projection = getCameraProjectionMatrix(aspect);
    auto view = getCameraViewMatrix();
    return projection * view;
}
```