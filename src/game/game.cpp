#include "game.hpp"

#include <exception>
#include <stdexcept>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader/shader.hpp"
#include "texture/texture.hpp"

Game::Game():
    player{glm::vec3(.5f, 1.5f, .5f), glm::vec3(1, 0, 0)}
{}
Game::~Game() {}

GLFWwindow* Game::getGLFWwindow() {
    return game_window;
}

Player& Game::getPlayer() {
    return player;
}

World& Game::getWorld() {
    return world;
}

void Game::gameloop() {
    glewExperimental = true; // Needed for core profile
    
    if (!glfwInit()) throw std::runtime_error("Unable to initialize GLFW!");

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
     // (In the accompanying source code, this variable is global for simplicity)
    game_window = glfwCreateWindow( 1300, 768, "MC Game", NULL, NULL);

    if (game_window == NULL) {
        throw std::runtime_error("Unable to create GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(game_window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile

    if (glewInit() != GLEW_OK) throw std::runtime_error("Unable to initialize GLEW");

    // OpenGL stuff
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Load the shaders for the game
    GLuint programID = LoadShaders(
        "/home/zachary/Desktop/mc-clone/resources/shaders/vertex.glsl",
        "/home/zachary/Desktop/mc-clone/resources/shaders/fragment.glsl");
    glUseProgram(programID);

    // Set some OpenGL settings
    // Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

    // Set background color
    glClearColor(.65f, .89f, 1.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(game_window, GLFW_STICKY_KEYS, GL_TRUE);

    // Visual settings
    const float aspect = 1300.f / 768.f;
    const float fov = 70.f;

    // ID for setting the MVP matrix
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // --- TEXTURE STUFF ---
    // Texture atlas for game
    GLuint Texture = LoadBMP("/home/zachary/Desktop/mc-clone/resources/Chunk.bmp");
	// ID for setting the texture
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

    // Generate world
    world.generateWorld(5, 5, 5);

    // Chunk buffers
    auto chunks = world.getChunksReference();

    // Generate test chunk and data
    // world.getBlock(glm::vec3(31, 31, 31)) = Block(Block::BlockType::AIR);

    // Buffers on the gpu
    // Render Loop
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.updatePlayer(*this);
        world.update();

        // Get the mvp from the player 
        auto MVP = player.getMVPmatrix(aspect, fov);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


        for (Chunk* chunk : chunks) {
            auto bufferInfo = chunk->getBufferInfo();
            // Draw
            // 1rst attribute buffer : vertices
            
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.vertexBuffer);
            glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, bufferInfo.uvBuffer);
            glVertexAttribPointer(
                1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
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


        // Swap buffers
        glfwSwapBuffers(game_window);
        glfwPollEvents();

    } while (glfwGetKey(game_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(game_window) == 0);

    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

    glfwTerminate();
}