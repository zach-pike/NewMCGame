#pragma once

#include <string>
#include <iostream>

class Logger {
public:
    enum class FGColors {
        BLACK   = 30,
        RED     = 31,
        GREEN   = 32,
        YELLOW  = 33,
        BLUE    = 34,
        MAGENTA = 35,
        CYAN    = 36,
        WHITE   = 37
    };

    enum class BGColors {
        BLACK   = 40,
        RED     = 41,
        GREEN   = 42,
        YELLOW  = 43,
        BLUE    = 44,
        MAGENTA = 45,
        CYAN    = 46,
        WHITE   = 47
    };

    enum class Styles {
        RESET      = 0,
        BOLD       = 1,
        DARKCOLOR  = 2,
        ITALIC     = 3,
        UNDERLINE  = 4,
        BLINKINGSL = 5,
        BLINKINGFT = 6,
        REVERSE    = 7,
        HIDE       = 8,
        CROSSOUT   = 9
    };
private:
    std::string prefix;
    FGColors color;

    void setFG(FGColors color);
    void setBG(BGColors fg);

    void style(Styles s);
public:
    Logger(std::string prefix, FGColors color);
    ~Logger();

    void info(std::string loginfo);
    void warn(std::string loginfo);
    void error(std::string loginfo);
};