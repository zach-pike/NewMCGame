#include "player.hpp"

#include "../game.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


Player::Player(glm::vec3 _position, glm::vec3 _looking):
    position(_position),
    looking(_looking) {}

Player::~Player() {}

void Player::updatePlayer(Game& game) {
    // Get screen width and height
    int width, height;
    glfwGetWindowSize(game.getGLFWwindow(), &width, &height);

    int hwidth = width / 2;
    int hheight = height / 2;

    // First, do the player look code
    static double pitch, yaw = 0;
    static double mx, my = 0.f;

    glfwGetCursorPos(game.getGLFWwindow(), &mx, &my);
    mx -= hwidth;
    my -= hheight;

    glfwSetCursorPos(game.getGLFWwindow(), hwidth, hheight);

    yaw += mx * .01;
    pitch += my * .01;

    looking = glm::vec3(cos(pitch)*cos(yaw), sin(pitch), sin(yaw) * cos(pitch));

    // Now player movement
    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
        position += looking;
    }

    if (glfwGetKey(game.getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS) {
        position -= looking;
    }
}

glm::mat4 Player::getMVPmatrix(float aspect, float fov) const {
    auto projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
    auto view = glm::lookAt(
		position, // Camera is at (4,3,3), in World Space
		position + looking, // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
    auto model = glm::mat4(1.f);

    return projection * view * model;
}