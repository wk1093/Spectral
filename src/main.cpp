#include "modules/win/module.h"
#include "modules/gfx/module.h"
#include "modules/shdr/module.h"

#include <cmath>

int main(int argc, char** argv) {
    const char* window_module = "glfw_gl";
    const char* graphics_module = "glad";
    if (argc == 3) {
        window_module = argv[1];
        graphics_module = argv[2];
    }

    WindowModule winm(window_module);
    if (!winm.lib.valid()) return 1;
    GraphicsModule gfxm(graphics_module);
    if (!gfxm.lib.valid()) return 1;

    ShaderModule shdr("spsl", gfxm.getShaderType());
    if (!shdr.lib.valid()) return 1;

    std::string window_title = "Test (win_" + std::string(window_module) + ", gfx_" + std::string(graphics_module) + ")";

    sWindow win = winm.loadWindow(window_title.c_str(), 800, 600);
    gfxm.init(&win);

    gfxm.setClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    sVertexDefinition* vertDef = gfxm.createVertexDefinition({3, 3, 3});

    // basic cube
    struct {
        float position[3];
        float normal[3];
        float color[3];
    } vertices[] = {
        // front
        {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
        // back
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
        // top
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
        // bottom
        {{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
        // right
        {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
        // left
        {{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}
    };
    sIndex indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    sShader vert{};
    sShader frag{};
    vert = shdr.compile(&gfxm, "spsl/basic.spslv", sShaderType::VERTEX, vertDef);
    frag = shdr.compile(&gfxm, "spsl/basic.spslf", sShaderType::FRAGMENT);
    sShaderProgram shader = gfxm.createShaderProgram({vert, frag});

    sMesh mesh = gfxm.createMesh(vert, vertices, sizeof(vertices), indices, sizeof(indices));

    sUniformDefinition uniformDef = {
        {sShaderType::FRAGMENT, "uColorMult", sUniformType::FLOAT, 3},
        {sShaderType::FRAGMENT, "uTime", sUniformType::FLOAT, 1},
        {sShaderType::VERTEX, "uProj", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uView", sUniformType::FLOAT, 4, 4},
    };

    sUniforms uniforms = gfxm.createUniforms(shader, uniformDef);
    static const float fov = 90.0f;
    static const float nearp = 0.1f;
    static const float farp = 100.0f;

    static const float matscale = 1 / tanf(fov * 0.5f * 3.14159f / 180.0f);

#pragma pack(push,1)
    struct ShaderData {
        float colorMult[3] = {1.0f, 1.0f, 1.0f};
        float time = 0.0f;
        float proj[4][4] = {
            {matscale, 0.0f, 0.0f, 0.0f},
            {0.0f, matscale, 0.0f, 0.0f},
            {0.0f, 0.0f, -farp / (farp-nearp), -1.0f},
            {0.0f, 0.0f, -(farp*nearp) / (farp-nearp), 1.0f}
        };
        // premade matrix to move the camera back (move everything forward)
        float view[4][4] = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, -20.0f},
            {0.0f, 0.0f, 1.0f, 1.0f}
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

        // shaderData.colorMult[0] = sinf(i++ * 0.1f) * 0.5f + 0.5f;
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
