#include "modules/win/module.h"
#include "modules/gfx/module.h"
#include "modules/shdr/module.h"

#include <cmath>

int main(int argc, char** argv) {
    const char* window_module = "eogll";
    const char* graphics_module = "eogll";
    if (argc == 3) {
        window_module = argv[1];
        graphics_module = argv[2];
    }

    WindowModule winm(window_module);
    if (!winm.lib.valid()) return 1;
    GraphicsModule gfxm(graphics_module);
    if (!gfxm.lib.valid()) return 1;

    ShaderModule shdr("spsl", gfxm.getShaderType()); // input format, then output format
    if (!shdr.lib.valid()) return 1;

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

    sUniformDefinition uniformDef = {
        {sShaderType::FRAGMENT, "uColorMult", sUniformType::FLOAT, 3},
        {sShaderType::FRAGMENT, "uTime", sUniformType::FLOAT, 1},
        {sShaderType::VERTEX, "uTestMatrix", sUniformType::FLOAT, 4, 4}
    };

    sUniforms uniforms = gfxm.createUniforms(shader, uniformDef);

#pragma pack(push,1)
    struct ShaderData {
        float colorMult[3] = {1.0f, 1.0f, 1.0f};
        float time = 0.0f;
        float testMatrix[4][4] = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
    } shaderData;
#pragma pack(pop)
    if (uniformDef.size() != sizeof(ShaderData)) {
        printf("ERROR: Uniform definition size does not match shader data size\n");
        printf("Uniform definition size: %d\n", uniformDef.size());
        printf("Shader data size: %d\n", sizeof(ShaderData));
        return 1;
    }

    unsigned int i = 0;

    while (!winm.shouldClose(win)) {
        winm.updateWindow(&win);
        gfxm.clear();

        shaderData.colorMult[0] = sinf(i++ * 0.1f) * 0.5f + 0.5f;
        shaderData.time = (float)winm.getTime(win);
        gfxm.useShaderProgram(shader);
        gfxm.setUniforms(uniforms, &shaderData);
        gfxm.drawMesh(mesh);

        gfxm.present();
        winm.swapBuffers(win);
    }

    winm.destroyWindow(win);

    return 0;
}
