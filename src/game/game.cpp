#include "game.hpp"

#include <exception>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader/shader.hpp"
#include "texture/texture.hpp"
#include "world/debug/ChunkBorderDebugger.hpp"
#include "utils/path.hpp"

template <typename T, typename... Args>
using FP = T(*)(Args...);

Game::Game():
    player{glm::vec3(.5f, 1.5f, .5f), glm::vec3(1, 0, 0), 70.f}
{
    gfxInit();

    std::cout << "WARNING: Make sure the CWD is the base path of the folder!!! Or else nothing will load correctly\n";

    // Generate world
    world.generateWorld(10, 5, 10);

    // VAO
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Shaders
    worldShader = loadShaders(
        getResourcePath("shaders/vertex.glsl"),
        getResourcePath("shaders/fragment.glsl")
    );
    lineDebugShader = loadShaders(
        getResourcePath("shaders/lineVertex.glsl"),
        getResourcePath("shaders/lineFragment.glsl")
    );
    
    // Textures
    textureAtlas = loadBMP(getResourcePath("Chunk.bmp"));

    // Load the plugins and enable them
    pluginManager.loadPlugins();

    // Resize the viewport on window size change
    glfwSetFramebufferSizeCallback(gameWindow, [](GLFWwindow* win, int w, int h) {
        glViewport(0, 0, w, h);
    });
}
Game::~Game() {
    pluginManager.unloadPlugins();

    // Cleanup
    glDeleteTextures(1, &textureAtlas);

    glDeleteShader(worldShader);
    glDeleteShader(lineDebugShader);

    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}
void Game::gfxInit() {
    // Init GLFW
    if (!glfwInit()) throw std::runtime_error("Unable to initialize GLFW!");

    // Window stuff
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Create GLFW window
    gameWindow = glfwCreateWindow(1300, 768, "MC Game", NULL, NULL);

    if (gameWindow == NULL) {
        throw std::runtime_error("Unable to create GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(gameWindow); // Initialize GLEW

    // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) throw std::runtime_error("Unable to initialize GLEW");

    // Set some OpenGL settings
    // Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

    // Set background color
    glClearColor(.65f, .89f, 1.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(gameWindow, GLFW_STICKY_KEYS, GL_TRUE);
}

void Game::gameLoop() {
    // ID for setting the MVP matrix
    GLuint worldMatrixID = glGetUniformLocation(worldShader, "MVP");
    GLuint debugMatrixID = glGetUniformLocation(lineDebugShader, "MVP");

    // --- TEXTURE STUFF ---
    // Texture atlas for game
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);
	glUniform1i(glGetUniformLocation(worldShader, "myTextureSampler"), 0);

    // Chunk buffers
    auto chunks = world.getChunksReference();
    
    // Debugger
    ChunkBorderDebugger chunkBorderDebugger;
    chunkBorderDebugger.draw(world);
    auto chunkBorderDrawingInfo = chunkBorderDebugger.getDrawingInfo();
    
    // ------------------------------

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetWindowSize(gameWindow, &windowWidth, &windowHeight);

        player.updatePlayer(*this);
        world.update();

        // Update the plugins
        pluginManager.pluginFrameUpdate(*this);

        // Get the mvp from the player 
        glUseProgram(worldShader);

        auto MVP = player.getMVPmatrix((float)windowWidth / (float)windowHeight);
        glUniformMatrix4fv(worldMatrixID, 1, GL_FALSE, &MVP[0][0]);
        for (Chunk* chunk : chunks) {
            auto bufferInfo = chunk->getBufferInfo();

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.vertexBuffer);
            glVertexAttribPointer(
                0,                  // attribute 0.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.uvBuffer);
            glVertexAttribPointer(
                1,                  // attribute 1.
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, chunk->getNVertices()); // 3 indices starting at 0 -> 1 triangle

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }

        if (player.showingDebug()) {
            glUseProgram(lineDebugShader);
            glUniformMatrix4fv(debugMatrixID, 1, GL_FALSE, &MVP[0][0]);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, chunkBorderDrawingInfo.vertexBuffer);
            glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, chunkBorderDrawingInfo.colorBuffer);
            glVertexAttribPointer(
                1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            glDrawArrays(GL_LINES, 0, chunkBorderDrawingInfo.nVerts);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }

        // Swap buffers
        glfwSwapBuffers(gameWindow);
        glfwPollEvents();
    } while (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(gameWindow) == 0); 
}

GLFWwindow* Game::getGLFWwindow() {
    return gameWindow;
}

Player& Game::getPlayer() {
    return player;
}

World& Game::getWorld() {
    return world;
}

int Game::getWindowWidth() const {
    return windowWidth;
}
int Game::getWindowHeight() const {
    return windowHeight;
}