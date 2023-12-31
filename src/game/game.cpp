#include "game.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "implot/implot.h"

#include <exception>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glHelpers/utils/dotenv.h"

template <typename T, typename... Args>
using FP = T(*)(Args...);

namespace chrono = std::chrono;

Game::Game():
    player{glm::vec3(1, 100, 1), glm::vec3(1, 0, 0), 70.f}
{
    // Load dotenv for shader and textures
    dotenv::init();
    std::cout << "WARNING: Make sure the CWD is the base path of the folder!!! Or else nothing will load correctly\n";

    // Initiate GLFW / glew / OpenGL
    gfxInit();

    // VAO
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Initializes all the shaders/textures/buffers
    world.gfxInit();
    billboardManager.gfxInit();
    hudTextManager.gfxInit();

    screenOverlays.gfxInit();

    // Load the mods and enable them
    modManager.loadMods();
    modManager.enableMods(*this);

    // Resize the viewport on window size change
    glfwSetFramebufferSizeCallback(gameWindow, [](GLFWwindow* win, int w, int h) {
        glViewport(0, 0, w, h);
    });
}
Game::~Game() {
    modManager.disableMods(*this);
    modManager.unloadMods();

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
    // Dear, Imgui setup
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(gameWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    // End setup


    // // Debugger
    // ChunkBorderDebugger chunkBorderDebugger(world);
    // ------------------------------

    // Stupid temp fix
    glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    bool debugOpen = false;

    float lastDrawTime = 0;

    long fc = 0;
    int maxChunksToDraw = 5;
    int drawFrequency = 2;

    int targetFramerate = 60.f;
    chrono::steady_clock::time_point frameEndTime;

    // Values for frame draw time graph
    bool frameGraphOpen = false;
    // We really don't change this much
    std::size_t bLen = 500;
    float* frameRateX = new float[bLen];
    for (int i=0; i < bLen; i++) frameRateX[i] = i;

    float* frameRateY = new float[bLen];
    memset(frameRateY, 0, sizeof(float) * bLen);
    
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto startTime = chrono::steady_clock::now();
        // Calculate delta T
        float deltaTime = (float)chrono::duration_cast<chrono::microseconds>(startTime - frameEndTime).count() / 1000.f;

        float frameTotalTime = 1000.f / (float)targetFramerate;


        // Start new Imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Shift framerate values back
        for(int i=0; i<(bLen-1); i++) frameRateY[i] = frameRateY[i + 1];
        frameRateY[bLen-1] = lastDrawTime;

        ImGui::Begin("Frame draw time", &frameGraphOpen);
        ImGui::Text("Last draw time %fms", lastDrawTime);

        if(ImPlot::BeginPlot("Frame draw time")) {
            ImPlot::PlotLine("Frame draw time", frameRateX, frameRateY, bLen);
            
            ImPlot::EndPlot();
        }
        ImGui::End();

        // On screen menu
        ImGui::Begin("Debug", &debugOpen, ImGuiWindowFlags_MenuBar);

        auto pos = player.getPositionRef();
        ImGui::Text("Player Position %f, %f, %f", pos.x, pos.y, pos.z);
        ImGui::Text("Polygon Count %ld", world.getVertexCount() / 3);
        ImGui::SliderInt("Target FPS", &targetFramerate, 1, 120);
        ImGui::SliderInt("Max number of chunks to draw per frame", &maxChunksToDraw, 1, 20);
        ImGui::SliderInt("Draw frequency", &drawFrequency, 1, 60);
        ImGui::SliderFloat("Max chunk draw dist", &maxViewDist, 0, 1000.f);
        ImGui::Text("Last draw time %fms", lastDrawTime);

        ImGui::End();

        glfwGetWindowSize(gameWindow, &windowWidth, &windowHeight);

        float aspect = (float)windowWidth / (float)windowHeight;

        player.updatePlayer(*this, deltaTime);
        if (fc % drawFrequency == 0) world.update(maxChunksToDraw);

        // Run the mods update function
        modManager.modFrameUpdate(*this);

        // 3d stuff
        auto viewMatrix = player.getCameraViewMatrix();
        auto viewProjection = player.getViewProjection(aspect);

        world.drawMeshes(viewProjection, player.getPositionRef(), maxViewDist);

        // if (player.showingDebug()) chunkBorderDebugger.draw(viewProjection);
        billboardManager.draw(player.getPositionRef(), viewMatrix, viewProjection);
        hudTextManager.draw(aspect);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        frameEndTime = chrono::steady_clock::now();
        lastDrawTime = (float)chrono::duration_cast<chrono::microseconds>(frameEndTime - startTime).count() / 1000.f;


        // Swap buffers
        glfwSwapBuffers(gameWindow);
        glfwPollEvents();

        // Frame rate locker
        float sleepTime = frameTotalTime - lastDrawTime;
        if (sleepTime > 0.f)
            std::this_thread::sleep_for(chrono::milliseconds((std::int64_t)sleepTime));
        
        fc ++;
    } while (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(gameWindow) == 0);

    delete[] frameRateX;
    delete[] frameRateY;

    // Destroy ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

Player& Game::getPlayerRef() {
    return player;
}

World& Game::getWorldRef() {
    return world;
}

ModManager& Game::getModManagerRef() {
    return modManager;
}

BillboardManager& Game::getBillboardManagerRef() {
    return billboardManager;
}

HUDTextManager& Game::getHUDTextManagerRef() {
    return hudTextManager;
}

GLFWwindow* Game::getGLFWwindow() {
    return gameWindow;
}

int Game::getWindowWidth() const {
    return windowWidth;
}
int Game::getWindowHeight() const {
    return windowHeight;
}
