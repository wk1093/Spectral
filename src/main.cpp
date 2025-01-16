#include "modules/win/module.h"
#include "modules/gfx/module.h"
#include "modules/shdr/module.h"

#include <cmath>

int main(int argc, char** argv) {


    const char* window_module;
    const char* graphics_module;
    bool use_dx = false;
    if (use_dx) {
        window_module = "glfw_noapi";
        graphics_module = "d3d11_1";
    } else {
        window_module = "glfw_gl";
        graphics_module = "glad";
    }
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
    struct Vertex {
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

    size_t vertSize = 0;
    for (int i = 0; i < vertDef->count; i++) {
        vertSize += vertDef->elements[i] * sizeof(float);
    }
    if (vertSize != sizeof(Vertex)) {
        printf("ERROR: Vertex definition size does not match vertex size\n");
        printf("Vertex definition size: %d\n", vertSize);
        printf("Vertex size: %d\n", sizeof(Vertex));
        return 1;
    }


    sIndex indices[] = {
        0, 1, 2, 2, 3, 0,
        6, 5, 4, 4, 7, 6,
        8, 9, 10, 10, 11, 8,
        14, 13, 12, 12, 15, 14,
        16, 17, 18, 18, 19, 16,
        22, 21, 20, 20, 23, 22
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
    static const float fov = 80.0f;
    static const float nearp = 0.001f;
    static const float farp = 1000.0f;
    static const float aspect_ratio = 800.0f / 600.0f;

    static const float matscale = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.0f);

#pragma pack(push,1)
    struct ShaderData {
        float colorMult[3] = {1.0f, 1.0f, 1.0f};
        float time = 0.0f;

        float proj[4][4] = {
            {matscale / aspect_ratio, 0.0f, 0.0f, 0.0f},
            {0.0f, matscale, 0.0f, 0.0f},
            {0.0f, 0.0f, -farp / (farp-nearp), -1.0f},
            {0.0f, 0.0f, -farp*nearp / (farp-nearp), 1.0f}
        };
        float view[4][4] = {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {-3.0f, -3.0f, -10.0f, 1.0f}
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

    float speed = 5.0f;

    winm.setMousePosition(win, 400, 300);

    while (!winm.shouldClose(win)) {
        winm.updateWindow(&win);
        gfxm.clear();

        float mousex, mousey;
        winm.getMousePosition(win, &mousex, &mousey);
        float dx = mousex - 400;
        float dy = mousey - 300;

        winm.setMousePosition(win, 400, 300);

        // Mouse look
        shaderData.view[3][0] += dx * 0.01f;
        shaderData.view[3][1] -= dy * 0.01f;
        
        if (winm.isKeyPressed(win, Key::Escape)) {
            winm.setShouldClose(win, true);
        }

        // WASD
        if (winm.isKeyPressed(win, Key::W)) {
            shaderData.view[3][2] += speed * win.dt;
        }
        if (winm.isKeyPressed(win, Key::S)) {
            shaderData.view[3][2] -= speed * win.dt;
        }
        if (winm.isKeyPressed(win, Key::A)) {
            shaderData.view[3][0] += speed * win.dt;
        }
        if (winm.isKeyPressed(win, Key::D)) {
            shaderData.view[3][0] -= speed * win.dt;
        }
        if (winm.isKeyPressed(win, Key::Q)) {
            shaderData.view[3][1] += speed * win.dt;
        }
        if (winm.isKeyPressed(win, Key::E)) {
            shaderData.view[3][1] -= speed * win.dt;
        }

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
