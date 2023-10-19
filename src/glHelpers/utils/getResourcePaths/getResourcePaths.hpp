#pragma once

#include <string>
#include <filesystem>

#include <stdlib.h>

namespace fs = std::filesystem;

// Get the full path to the Vertex and Fragment shader
std::pair<std::string, std::string> getShaderPaths(std::string shaderFolder);

std::string getTexturePath(std::string p);