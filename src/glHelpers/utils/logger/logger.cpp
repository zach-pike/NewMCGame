/*
private:
    std::string prefix;

    void changeFG(FGColors color);
    void changeBG(FGColors color);

    void style(Styles s);
public:
    Logger(std::string prefix, FGColors color);
    ~Logger();

    void info();
    void warn();
    void error();
};
*/

#include "logger.hpp"

Logger::Logger(std::string pfx, Logger::FGColors col):
    prefix(pfx),
    color(col) {}

Logger::~Logger() {}

void Logger::setFG(FGColors fg) {
    std::cout << "\033[" << (int)fg << 'm';
}
void Logger::setBG(BGColors bg) {
    std::cout << "\033[" << (int)bg << 'm';
}

void Logger::style(Styles s) {
    std::cout << "\033[" << (int)s << 'm';
}

void Logger::info(std::string loginfo) {
    setFG(color);
    std::cout << "[" << prefix << "] ";
    style(Styles::RESET); 
    std::cout << loginfo << '\n';
}

void Logger::warn(std::string loginfo) {
    setFG(color);
    std::cout << "[" << prefix << "] ";
    setFG(FGColors::YELLOW);
    std::cout << loginfo << '\n';
    style(Styles::RESET);
}

void Logger::error(std::string loginfo) {
    setFG(color);
    std::cout << "[" << prefix << "] ";
    setFG(FGColors::RED);
    std::cout << loginfo << '\n';
    style(Styles::RESET); 
}