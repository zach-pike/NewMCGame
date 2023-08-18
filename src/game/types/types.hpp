#pragma once

#include <GL/glew.h>

struct Vertex {
    GLfloat x, y, z;
} __attribute__((packed));

struct UV {
    GLfloat u, v;
} __attribute__((packed));