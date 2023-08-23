#include "pluginManager/include/IPlugin.hpp"
#include "game.hpp"

class XBoxJoystickPlugin : public IPlugin {
public:
    void setup(Game&);
    void frameUpdate(Game&);
    void cleanup(Game&);
    std::string getPluginName();
    Version getPluginVersion();
};

extern "C" {
    IPlugin* create();
    void destroy(IPlugin* plugin);
}