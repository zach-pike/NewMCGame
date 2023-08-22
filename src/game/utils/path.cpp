#include "path.hpp"

std::string getFullPath(std::string path) {
    std::string nPath = std::filesystem::current_path();
    nPath += path;
    return nPath;
}

// Get Path to something in the resources folder
std::string getResourcePath(std::string path) {
    return getFullPath("/resources/" + path);
}