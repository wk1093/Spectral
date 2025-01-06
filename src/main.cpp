#include "modules/scrld/module.h"
#include "modules/win/module.h"
#include "modules/gfx/module.h"

#include <iostream>

int main() {
    WindowModule winm("eogll");
    if (!winm.lib.valid()) {
        printf("Error loading window module\n");
        return 1;
    }

    GraphicsModule gfxm("eogll");
    if (!gfxm.lib.valid()) {
        printf("Error loading graphics module\n");
        return 1;
    }

    sWindow win = winm.loadWindow("test", 800, 600);
    gfxm.init(&win);

    gfxm.setClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    while (!winm.shouldClose(win)) {
        winm.updateWindow(win);
        gfxm.clear();

        if (winm.isKeyPressed(win, Key::A)) {
            printf("A pressed\n");
        }

        gfxm.debugTriangle();

        winm.swapBuffers(win);
    }

    winm.destroyWindow(win);

    return 0;
}
