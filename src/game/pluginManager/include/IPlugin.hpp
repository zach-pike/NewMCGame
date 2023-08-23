#pragma once

#include <string>

class Game;

class IPlugin {
public:
    struct Version {
        int major, minor;
    };

    virtual void setup(Game&) = 0;
    virtual void frameUpdate(Game& game) = 0;
    virtual void cleanup(Game&) = 0;

    virtual std::string getPluginName() = 0;
    virtual Version getPluginVersion() = 0;
};

template <typename T, typename... Args>
using FP = T(*)(Args...);

using PluginCreatorFunction = FP<IPlugin*>;
using PluginDestroyerFunction = FP<void, IPlugin*>;