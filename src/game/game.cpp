#include "game.hpp"

#include <exception>
#include <stdexcept>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader/shader.hpp"
#include "texture/texture.hpp"

Game::Game() {}
Game::~Game() {}

void Game::gameloop() {
    glewExperimental = true; // Needed for core profile
    
    if (!glfwInit()) {
        throw std::runtime_error("Unable to initialize GLFW!");
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
     // (In the accompanying source code, this variable is global for simplicity)
    game_window = glfwCreateWindow( 1024, 768, "MC Game", NULL, NULL);

    if (game_window == NULL) {
        throw std::runtime_error("Unable to create GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(game_window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Unable to initialize GLEW");
    }

    // OpenGL stuff
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders(
        "/home/zachary/Desktop/mc-clone/resources/shaders/vertex.glsl",
        "/home/zachary/Desktop/mc-clone/resources/shaders/fragment.glsl");
    glUseProgram(programID);

    glClearColor(.65f, .89f, 1.f, 1.f);
    glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	static const GLfloat g_vertex_buffer_data[] = {
        -1.f, -1.f,
         1.f,  1.f,
         1.f, -1.f
     };

	// Two UV coordinatesfor each vertex. They were created with Blender.
	static const GLfloat g_uv_buffer_data[] = {
        0.f, 1.f,
        1.f, 1.f,
        1.f, 0.f
    };

    glm::mat4 View = glm::lookAt(
		glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);


	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    GLuint Texture = LoadBMP("/home/zachary/Desktop/mc-clone/resources/Chunk.bmp");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

    GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(game_window, GLFW_STICKY_KEYS, GL_TRUE);
    do {
        glClear(GL_COLOR_BUFFER_BIT);

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
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(game_window);
        glfwPollEvents();

    } while (glfwGetKey(game_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(game_window) == 0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

    glfwTerminate();
}