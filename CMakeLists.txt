cmake_minimum_required(VERSION 3.22.1)

project(mc_clone VERSION 1.0)

file(GLOB_RECURSE GAME_SRC src/game/*.cpp)
file(GLOB XBOXJOY_SRC src/game/plugins/modules/XBoxJoystick/*.cpp)

include_directories(src/game)

# Main executable
add_executable(main src/main.cpp)

# Minecraft Game library
add_library(gameLib SHARED ${GAME_SRC})

# Joystick library
add_library(XBoxJoystickLib SHARED ${XBOXJOY_SRC})

# Set the output location of the shared object
set_target_properties(XBoxJoystickLib
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "../resources/cplugins"
)

# Link libaries for gameLib
target_link_libraries(gameLib GL GLEW glfw m X11 Xi Xrandr dl)

# Link gameLib
link_libraries(${CMAKE_BINARY_DIR}/libgameLib.so)

# Link for main executable
target_link_libraries(main gameLib)
target_link_libraries(XBoxJoystickLib gameLib)