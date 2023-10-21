#include "getEnvVar.hpp"

std::string getEnvironmentVar(const char* c) {
    char* a = getenv(c);

    if (a != nullptr) return std::string(a);
    else return std::string("");
}