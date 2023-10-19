#include "getResourcePaths.hpp"

static std::string getEnviromentVar(const char* c) {
    char* a = getenv(c);

    if (a != nullptr) return std::string(a);
    else return std::string("");
}

std::pair<std::string, std::string> getShaderPaths(std::string shaderFolder) {
    // Get base path
    auto bp = fs::current_path();

    auto sf = getEnviromentVar("SHADERS_FOLDER");

    auto vtxPath =  bp / fs::path(sf) / fs::path(shaderFolder) / fs::path("vertex.glsl");
    auto fragPath = bp / fs::path(sf) / fs::path(shaderFolder) / fs::path("fragment.glsl");

    return std::pair(vtxPath, fragPath);
}

std::string getTexturePath(std::string p) {
    auto bp = fs::current_path();

    auto shaderFolder = getEnviromentVar("TEXTURES_FOLDER");
    auto path = bp / shaderFolder / fs::path(p);

    return path;
}