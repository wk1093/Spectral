#include "modules/win/module.h"
#include "modules/gfx/module.h"

int main() {
    WindowModule winm("sf");
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

    sVertex vertices[] = {
            {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
    sIndex indices[] = {
            0, 1, 2
    };

    sMesh mesh = gfxm.createMesh(vertices, sizeof(vertices), indices, sizeof(indices));

    sShader vert = gfxm.loadShader("shaders/vert.glsl", sShaderType::VERTEX);
    sShader frag = gfxm.loadShader("shaders/frag.glsl", sShaderType::FRAGMENT);
    sShaderProgram shader = gfxm.createShaderProgram({vert, frag});

    while (!winm.shouldClose(win)) {
        winm.updateWindow(win);
        gfxm.clear();

        if (winm.isKeyPressed(win, Key::A)) {
            printf("A pressed\n");
        }

        gfxm.useShaderProgram(shader);
        gfxm.drawMesh(mesh);

        winm.swapBuffers(win);
    }

    winm.destroyWindow(win);

    return 0;
}
