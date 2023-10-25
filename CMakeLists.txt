cmake_minimum_required(VERSION 3.22.1)

project(MCGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source file for the game && helper functions
file(GLOB_RECURSE GAME_SRC src/game/*.cpp)
file(GLOB_RECURSE HELPER_SRC src/glHelpers/*.cpp)

file(GLOB XBOXJOY_SRC resources/mods/XBoxJoystick/*.cpp)
file(GLOB WORLDINFOSIGN_SRC resources/mods/WorldInfoSign/*.cpp)
file(GLOB_RECURSE WORLDGEN_SRC resources/mods/WorldGen/*.cpp)

include_directories(src/game)

# Plugins
add_library(XBoxJoystickLib SHARED ${XBOXJOY_SRC})
add_library(WorldInfoSignLib SHARED ${WORLDINFOSIGN_SRC})
add_library(WorldGen SHARED ${WORLDGEN_SRC})

# Set the output location of the plugins
set_target_properties(XBoxJoystickLib
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "../resources/cmods"
)
set_target_properties(WorldInfoSignLib
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "../resources/cmods"
)
set_target_properties(WorldGen
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "../resources/cmods"
)

# Stuff for external dependencies
# Dear, ImGui
include_directories(src/)

# Hate this hack but it works
include_directories(external/)
include_directories(external/imgui)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB IMGUI_SRC external/imgui/*.cpp)
set(IMGUI_IMPL_SRC external/imgui/backends/imgui_impl_glfw.cpp external/imgui/backends/imgui_impl_opengl3.cpp)

# Main executable
add_executable(main src/main.cpp)

# Minecraft Game library
add_library(gameLib SHARED ${GAME_SRC} ${HELPER_SRC} ${IMGUI_SRC} ${IMGUI_IMPL_SRC})
target_link_libraries(gameLib GL GLEW glfw m X11 Xi Xrandr dl)

# Link gameLib
link_libraries(${CMAKE_BINARY_DIR}/libgameLib.so)

# Link the main executable to the gameLib
target_link_libraries(main gameLib)

# Link the game library to the plugins
target_link_libraries(XBoxJoystickLib gameLib)
target_link_libraries(WorldInfoSignLib gameLib)
target_link_libraries(WorldGen gameLib)