#include "getEnvVar.hpp"

std::string getEnviromentVar(const char* c) {
    char* a = getenv(c);

    if (a != nullptr) return std::string(a);
    else return std::string("");
}