#include "basicGfxInit.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <stdexcept>

GLFWwindow* gfxInit() {
    // Init GLFW
    if (!glfwInit()) throw std::runtime_error("Unable to initialize GLFW!");

    // Window stuff
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Create GLFW window
    GLFWwindow* win = glfwCreateWindow(768, 768, "Spirals", NULL, NULL);

    if (win == NULL) {
        throw std::runtime_error("Unable to create GLFW window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(win);

    // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) throw std::runtime_error("Unable to initialize GLEW");

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);

    return win;
}
