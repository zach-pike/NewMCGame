cmake_minimum_required(VERSION 3.22.1)

project(mc_clone VERSION 1.0)

file(GLOB_RECURSE GAME_SRC src/game/*.cpp)
file(GLOB XBOXJOY_SRC resources/plugins/XBoxJoystick/*.cpp)
file(GLOB WORLDINFOSIGN_SRC resources/plugins/WorldInfoSign/*.cpp)
file(GLOB EXTRADEBUG_SRC resources/plugins/ExtraDebug/*.cpp)

include_directories(src/game)

# Main executable
add_executable(main src/main.cpp)

# Minecraft Game library
add_library(gameLib SHARED ${GAME_SRC})

# Plugins
add_library(XBoxJoystickLib SHARED ${XBOXJOY_SRC})
add_library(WorldInfoSignLib SHARED ${WORLDINFOSIGN_SRC})
add_library(ExtraDebugLib SHARED ${EXTRADEBUG_SRC})

# Set the output location of the plugins
set_target_properties(XBoxJoystickLib
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "../resources/cplugins"
)
set_target_properties(WorldInfoSignLib
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "../resources/cplugins"
)
set_target_properties(ExtraDebugLib
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "../resources/cplugins"
)

# Link libaries for gameLib
target_link_libraries(gameLib GL GLEW glfw m X11 Xi Xrandr dl)

# Link gameLib
link_libraries(${CMAKE_BINARY_DIR}/libgameLib.so)

# Link the main executable to the gameLib
target_link_libraries(main gameLib)

# Link the game library to the plugins
target_link_libraries(XBoxJoystickLib gameLib)
target_link_libraries(WorldInfoSignLib gameLib)
target_link_libraries(ExtraDebugLib gameLib)