#include "pluginManager/include/IPlugin.hpp"
#include "game.hpp"

class XBoxJoystickPlugin : public IPlugin {
public:
    XBoxJoystickPlugin();
    ~XBoxJoystickPlugin();

    void setup();
    void frameUpdate(Game&);
    void cleanup();
    std::string getPluginName();
};

extern "C" {
    IPlugin* create();
    void destroy(IPlugin* plugin);
}