#include "game.hpp"

#include <exception>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <thread>
#include <filesystem>
#include <dlfcn.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader/shader.hpp"
#include "texture/texture.hpp"
#include "world/debug/ChunkBorderDebugger.hpp"

namespace fs = std::filesystem;

template <typename T, typename... Args>
using FP = T(*)(Args...);

Game::Game():
    player{glm::vec3(.5f, 1.5f, .5f), glm::vec3(1, 0, 0), 70.f}
{
    gfxInit();

    // Generate world
    world.generateWorld(10, 5, 10);

    // VAO
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Shaders
    worldShader = LoadShaders(
        "/home/zachary/Desktop/mc-clone/resources/shaders/vertex.glsl",
        "/home/zachary/Desktop/mc-clone/resources/shaders/fragment.glsl");
    lineDebugShader = LoadShaders(
        "/home/zachary/Desktop/mc-clone/resources/shaders/lineVertex.glsl",
        "/home/zachary/Desktop/mc-clone/resources/shaders/lineFragment.glsl");
    
    // Textures
    textureAtlas = LoadBMP("/home/zachary/Desktop/mc-clone/resources/Chunk.bmp");

    // Load plugins
    loadPlugins();
}
Game::~Game() {
    unloadPlugins();

    // Cleanup
    glDeleteTextures(1, &textureAtlas);

    glDeleteShader(worldShader);
    glDeleteShader(lineDebugShader);

    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();
}

void Game::loadPlugins() {
    // Look in the resources/cplugins folder and get all the files
    auto paths = fs::directory_iterator("/home/zachary/Desktop/mc-clone/resources/cplugins/");

    // Open them all first
    for (auto path : paths) {
        std::string ldpath = path.path();

        void* pluginHandle = dlopen(ldpath.c_str(), RTLD_NOW);

        if (pluginHandle == nullptr) {
            std::string error = "Failed to load plugin " + ldpath + " Error \"";
            error.append(dlerror());
            throw std::runtime_error(error);
        }
        pluginHandles.push_back(pluginHandle);
    }

    // Create all the plugin instances
    for (void* pluginHandle : pluginHandles) {
        auto createFn = (FP<IPlugin*>)dlsym(pluginHandle, "create");
        auto destroyFn = (FP<void, IPlugin*>)dlsym(pluginHandle, "destroy");

        auto plugin = std::unique_ptr<IPlugin, PluginDestroyerFunction>(createFn(), destroyFn);
        std::string name = plugin->getPluginName();
        plugins.push_back(std::move(plugin));
        std::cout << "[Plugins] Loaded plugin " << name << '\n';
    }

    // Run all the setup functions
    for (auto& plugin : plugins) {
        plugin->setup(*this);
    }
}

void Game::unloadPlugins() {
    // Call the cleanup function on all plugins
    for(auto& plugin : plugins) {
        plugin->cleanup(*this);
    }

    plugins.clear(); // This will destroy all the Plugin Classes with their destructor

    // Unload all the open libraries
    for (void* pluginHandle : pluginHandles) {
        dlclose(pluginHandle);
    }

    // Get rid of the plugin handles
    pluginHandles.clear();
}
void Game::pluginFrameUpdate() {
    for (auto& plugin : plugins) {
        plugin->frameUpdate(*this);
    }
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
    // Visual settings
    const float aspect = 1300.f / 768.f;

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
    
    ChunkBorderDebugger chunkBorderDebugger;
    chunkBorderDebugger.draw(world);
    auto chunkBorderDrawingInfo = chunkBorderDebugger.getDrawingInfo();
    
    // ------------------------------

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.updatePlayer(*this);
        world.update();

        // Update the plugins
        pluginFrameUpdate();

        // Get the mvp from the player 
        glUseProgram(worldShader);

        auto MVP = player.getMVPmatrix(aspect);
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
