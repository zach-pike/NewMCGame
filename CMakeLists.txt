cmake_minimum_required(VERSION 3.22.1)

project(mc_clone VERSION 1.0)

file(GLOB_RECURSE SRC src/*.cpp)

add_executable(main ${SRC})

target_link_libraries(main GL GLEW glfw m X11 Xi Xrandr)