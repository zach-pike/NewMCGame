#include "ModManager/include/IMod.hpp"
#include "game.hpp"

class XBoxJoystickPlugin : public IMod {
public:
    void setup(Game&);
    void frameUpdate(Game&);
    void cleanup(Game&);
    std::string getModName();
    Version getModVersion();
};

extern "C" {
    XBoxJoystickPlugin* create();
    void destroy(XBoxJoystickPlugin* plugin);
}