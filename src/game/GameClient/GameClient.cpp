#include "GameClient.hpp"

#include "utils/dotenv.h"
#include "utils/logger/logger.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

GameClient::GameClient() {
    dotenv::init();
}
GameClient::~GameClient() {}

void GameClient::_networkingThread() {
    networkingThreadRunning = true;
    Logger logger("Networking Thread", Logger::FGColors::CYAN);
    // Networking setup

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in socketAddress;
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &socketAddress.sin_addr) <= 0) {
        logger.error("Could not parse hostname \"" + host + "\"");
        networkingThreadRunning = false;
        return;
    }

    if (connect(sock, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
        logger.error("Could not connect to host");
        networkingThreadRunning = false;
        return;
    }

    while(networkingThreadRunning) {
        // Networking loop

        if (outboundMessages.messageAvailable()) {
            auto msg = outboundMessages.getMessage();

            // Convert message into flat data
            std::size_t bufferSize = sizeof(std::uint64_t) + msg.getSize();
            std::uint8_t* buffer = new std::uint8_t[bufferSize];

            // Write packet size
            *(std::uint64_t*)buffer = msg.getSize();
            // Write packet type
            *(ClientServerMessageTypes*)(buffer + sizeof(std::uint64_t)) = msg.msgType;

            // Copy message content
            memcpy(buffer + sizeof(uint64_t) + sizeof(ClientServerMessageTypes),
                   msg.messageContent.data(),
                   msg.messageContent.size());
            
            // Done! buffer now contains our whole message
            // time to write
            write(sock, buffer, bufferSize);
        }
    }

    close(sock);
}

void GameClient::_gfxThread() {
    gfxThreadRunning = true;
    Logger logger("Graphics Thread", Logger::FGColors::MAGENTA);
    // ----------------- GFX setup -----------------

    // Init GLFW
    if (!glfwInit()) throw std::runtime_error("Unable to initialize GLFW!");

    // Window stuff
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 


    GLFWwindow* window = glfwCreateWindow(1300, 768, "MC Game", NULL, NULL);

    if (window == NULL) {
        throw std::runtime_error("Unable to create GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

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
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // ---------------------------------------------

    // Resize the viewport on window size change
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h) {
        glViewport(0, 0, w, h);
    });

    // Setup VAO
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    while(gfxThreadRunning) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // DRAW HERE


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) || glfwWindowShouldClose(window)) gfxThreadRunning = false;
    }
}

void GameClient::startNetworking() {
    networkingThread = std::jthread(std::bind(&GameClient::_networkingThread, this));
}

void GameClient::startupGfx() {
    gfxThread = std::jthread(std::bind(&GameClient::_gfxThread, this));
}