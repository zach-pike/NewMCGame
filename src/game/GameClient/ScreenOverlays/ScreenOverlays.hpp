#pragma once

class ScreenOverlays {
private:
    void drawBuffers();
public:
    ScreenOverlays();
    ~ScreenOverlays();

    void gfxInit();

    void glDraw();
};