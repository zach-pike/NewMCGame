#include "game.hpp"

#include <exception>
#include <stdexcept>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader/shader.hpp"
#include "texture/texture.hpp"
#include "world/chunk/chunk.hpp"

Game::Game():
    player{glm::vec3(.5f, 1.5f, .5f), glm::vec3(1, 0, 0)} {}
Game::~Game() {}

GLFWwindow* Game::getGLFWwindow() {
    return game_window;
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
    const float fov = 45.f;

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

    // Data to be buffered
    std::vector<Vertex> vertex_data;
    std::vector<UV> uv_data;

    // Generate test chunk and data
    Chunk chunk;
    chunk.generateVertices(vertex_data, uv_data);

    // Buffers on the gpu
    GLuint vertexbuffer, uvbuffer;

    // Generate and upload the verticies
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(Vertex), vertex_data.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uv_data.size() * sizeof(UV), uv_data.data(), GL_STATIC_DRAW);

    // Render Loop
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.updatePlayer(*this);

        // Get the mvp from the player 
        auto MVP = player.getMVPmatrix(aspect, fov);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Draw
        // 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
		    1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertex_data.size() * 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(game_window);
        glfwPollEvents();

    } while (glfwGetKey(game_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(game_window) == 0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

    glfwTerminate();
}