#pragma once

#include <string>
#include <filesystem>

// Make sure the path starts with a slash!!!
std::string getFullPath(std::string path);

// Get Path to something in the resources folder
std::string getResourcePath(std::string path);