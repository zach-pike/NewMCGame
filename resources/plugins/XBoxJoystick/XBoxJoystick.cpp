#include "XBoxJoystick.hpp"
#include "GLFW/glfw3.h"

#include "player/ray/ray.hpp"

#include <iostream>
#include <cmath>

#define MOVE_AXES_X 0
#define MOVE_AXES_Y 1

#define LOOK_AXES_X 2
#define LOOK_AXES_Y 3

#define RIGHT_TRIGGER_AXES 4

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 2
#define BUTTON_Y 3

IPlugin* create() {
    return new XBoxJoystickPlugin();
}

void destroy(IPlugin* plugin) {
    delete plugin;
}

XBoxJoystickPlugin::XBoxJoystickPlugin() {}
XBoxJoystickPlugin::~XBoxJoystickPlugin() {}

const float sens = 0.05f;
const float moveSpeed = .25f;
const float cameraPitchMax = M_PI * 1.49;
const float cameraPitchMin = M_PI_2 + .01f;

void XBoxJoystickPlugin::setup() {
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);

        std::cout << "[XBoxJoystickPlugin] Found controller " << name << '\n';
    }
}

void XBoxJoystickPlugin::frameUpdate(Game& game) {
    if (!glfwJoystickPresent(GLFW_JOYSTICK_1)) return;

    int axesCount;
    int buttonCount;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

    float moveAxisX = axes[MOVE_AXES_X];
    float moveAxisY = axes[MOVE_AXES_Y];

    float fixedMoveAxisX = fabs(moveAxisX) > .2 ? moveAxisX : 0;
    float fixedMoveAxisY = fabs(moveAxisY) > .2 ? moveAxisY : 0;

    float lookAxisX = axes[LOOK_AXES_X];
    float lookAxisY = axes[LOOK_AXES_Y];

    static float cameraPitch = 2.8f;
    static float cameraYaw = 0;

    if (fabs(lookAxisX) > .2) {
        cameraYaw += lookAxisX * sens;
    }

    if (fabs(lookAxisY) > .2) {
        cameraPitch = std::min(cameraPitchMax, std::max(cameraPitchMin, cameraPitch + lookAxisY * sens));
    }

    Player& p = game.getPlayer();
    glm::vec3& looking = p.getLookingVectorReference();
    glm::vec3& position = p.getPositionReference();

    glm::vec3 newLookVec = glm::vec3(cos(cameraPitch)*cos(cameraYaw), sin(cameraPitch), sin(cameraYaw) * cos(cameraPitch));
    looking = newLookVec;

    glm::vec3 moveVec = looking * (-1.f * fixedMoveAxisY) * moveSpeed
                       + glm::vec3(looking.z, 0, -looking.x) * (-1.f * fixedMoveAxisX) * moveSpeed;

    position += moveVec;

    if (buttons[BUTTON_A]) {
        position += glm::vec3(0, 1, 0) * moveSpeed;
    }
    if (buttons[BUTTON_B]) {
        position -= glm::vec3(0, 1, 0) * moveSpeed;
    }

    static bool breakTriggerPressed = false;
    if (axes[RIGHT_TRIGGER_AXES] > .75f) {
        if (breakTriggerPressed != true) {
            breakTriggerPressed = true;

            Ray r(position, looking);
            r.tryBreakBlock(game.getWorld());
        }
    } else {
        breakTriggerPressed = false;
    }
}

void XBoxJoystickPlugin::cleanup() {}

std::string XBoxJoystickPlugin::getPluginName() {
    return "XBoxJoystickPlugin VERSION 1.0";
}