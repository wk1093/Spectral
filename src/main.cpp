#include "modules/win/module.h"
#include "modules/gfx/module.h"
#include "modules/shdr/module.h"

int main(int argc, char** argv) {
    const char* window_module = "glfw_noapi";
    const char* graphics_module = "d3d11_1";
    if (argc == 3) {
        window_module = argv[1];
        graphics_module = argv[2];
    }

    WindowModule winm(window_module);
    if (!winm.lib.valid()) {
        printf("Error loading window module\n");
        return 1;
    }
    GraphicsModule gfxm(graphics_module);
    if (!gfxm.lib.valid()) {
        printf("Error loading graphics module\n");
        return 1;
    }

    ShaderModule shdr("spsl", gfxm.getShaderType()); // input format, then output format
    if (!shdr.lib.valid()) {
        printf("Error loading shader module\n");
        return 1;
    }

    std::string window_title = "Test (win_" + std::string(window_module) + ", gfx_" + std::string(graphics_module) + ")";

    sWindow win = winm.loadWindow(window_title.c_str(), 800, 600);
    gfxm.init(&win);

    gfxm.setClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    sVertexDefinition* vertDef = gfxm.createVertexDefinition({3, 3});

    struct {
        float position[3];
        float color[3];
    } vertices[] = {
            {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
    sIndex indices[] = {
            0, 1, 2
    };
    sShader vert{};
    sShader frag{};
    vert = shdr.compile(&gfxm, "spsl/basic.spslv", sShaderType::VERTEX, vertDef);
    frag = shdr.compile(&gfxm, "spsl/basic.spslf", sShaderType::FRAGMENT, vertDef);
    sShaderProgram shader = gfxm.createShaderProgram({vert, frag});

    sMesh mesh = gfxm.createMesh(vert, vertices, sizeof(vertices), indices, sizeof(indices));

    while (!winm.shouldClose(win)) {
        winm.updateWindow(win);
        gfxm.clear();

        gfxm.useShaderProgram(shader);
        gfxm.drawMesh(mesh);

        gfxm.present();
        winm.swapBuffers(win);
    }

    winm.destroyWindow(win);

    return 0;
}
