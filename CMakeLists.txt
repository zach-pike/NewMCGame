cmake_minimum_required(VERSION 3.22.1)

project(MCGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src/game)

# Source file for the game server && client
file(GLOB_RECURSE CLIENT_SRC src/game/GameClient/*.cpp)
file(GLOB_RECURSE SERVER_SRC src/game/GameServer/*.cpp)

# ----- External stuff -----
include_directories(external/)
include_directories(external/imgui)

# External library files
file(GLOB IMGUI_SRC external/imgui/*.cpp)
set(IMGUI_IMPL_SRC external/imgui/backends/imgui_impl_glfw.cpp external/imgui/backends/imgui_impl_opengl3.cpp)

file(GLOB IMPLOT_SRC external/implot/*.cpp)

# --------------------------

# General Utilities libraries
file(GLOB_RECURSE UTILS_SRC src/game/utils/*.cpp)

# Files with main functions for individual programs
set(CLIENT_MAIN_FILE src/GameClientMain.cpp)
set(SERVER_MAIN_FILE src/GameServerMain.cpp)

# Create the game client library and link gl glew glfw ect
add_library(gameClient SHARED ${CLIENT_SRC} ${IMGUI_SRC} ${IMGUI_IMPL_SRC} ${IMPLOT_SRC})
target_link_libraries(gameClient GL GLEW glfw m) 

# Create the game server library
add_library(gameServer SHARED ${SERVER_SRC})
target_link_libraries(gameServer dl ssl crypto)

# Create the executables

# Client
add_executable(gameClientPrgm ${CLIENT_MAIN_FILE} ${UTILS_SRC})
target_link_libraries(gameClientPrgm gameClient)

# Server
add_executable(gameServerPrgm ${SERVER_MAIN_FILE} ${UTILS_SRC})
target_link_libraries(gameServerPrgm gameServer)

# ----- MODS -------

# Get mod files
# file(GLOB WORLDINFOSIGN_SRC resources/mods/WorldInfoSign/*.cpp)
# file(GLOB_RECURSE WORLDGEN_SRC resources/mods/WorldGen/*.cpp)

# # Create the targets
# add_library(WorldInfoSignLib SHARED ${WORLDINFOSIGN_SRC})
# add_library(WorldGen SHARED ${WORLDGEN_SRC})

# # Set library output directory
# set_target_properties(WorldInfoSignLib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../resources/cmods")
# set_target_properties(WorldGen PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../resources/cmods")

# # Link mods with server so they have access to all the functions
# target_link_libraries(WorldInfoSignLib gameServer)
# target_link_libraries(WorldGen gameServer)
