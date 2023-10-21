#pragma once

#include <string>

class Game;

class IMod {
public:
    struct Version {
        int major, minor;
    };

    virtual void setup(Game&) = 0;
    virtual void frameUpdate(Game& game) = 0;
    virtual void cleanup(Game&) = 0;

    virtual std::string getModName() = 0;
    virtual Version getModVersion() = 0;
};

template <typename T, typename... Args>
using FP = T(*)(Args...);

using ModCreateFunction = FP<IMod*>;
using ModDestroyerFunction = FP<void, IMod*>;