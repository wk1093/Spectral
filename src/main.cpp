#include "modules/win/module.h"
#include "modules/gfx/module.h"
#include "modules/shdr/module.h"
#include "modules/math/module.h" // doesn't need to be loaded, because all the functions are inlined and the module is header-only

#include "stb_image.h"

#include <cmath>

sTextureDefinition loadTexture(const char* path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == nullptr) {
        printf("Error loading texture\n");
        return {0, 0, 0, nullptr};
    }
    if (width <= 0 || height <= 0 || channels <= 0) {
        printf("Error loading texture\n");
        return {0, 0, 0, nullptr};
    }
    return {static_cast<size_t>(width), static_cast<size_t>(height), static_cast<size_t>(channels), data};
}

int main(int argc, char** argv) {
    const char* window_module;
    const char* graphics_module;
    bool use_dx = true;
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

    gfxm.setClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    sTextureDefinition texDef = loadTexture("textures/test.png");
    sTexture tex = gfxm.createTexture(texDef);

    sVertexDefinition* vertDef = gfxm.createVertexDefinition({3, 3, 2});

    // basic cube
    struct Vertex {
        float position[3];
        float normal[3];
        float texcoord[2];
    } vertices[] = {
        {{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
        {{-1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},

        {{-1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},

        {{-1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
        {{-1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},

        {{-1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
        {{-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},

        {{ 1.0f, -1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{ 1.0f,  1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},

        {{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}}
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
         0,  1,  2,   2,  3,  0,
         6,  5,  4,   4,  7,  6,
         8,  9, 10,  10, 11,  8,
        14, 13, 12,  12, 15, 14,
        16, 17, 18,  18, 19, 16,
        22, 21, 20,  20, 23, 22
    };
    sShader vert{};
    sShader frag{};
    vert = shdr.compile(&gfxm, "spsl/basic.spslv", sShaderType::VERTEX, vertDef);
    frag = shdr.compile(&gfxm, "spsl/basic.spslf", sShaderType::FRAGMENT);
    sShaderProgram shader = gfxm.createShaderProgram({vert, frag});

    sMesh mesh = gfxm.createMesh(vert, vertices, sizeof(vertices), indices, sizeof(indices));

    sUniformDefinition uniformDef = {
        {sShaderType::FRAGMENT, "uTime", sUniformType::FLOAT, 1},
        {sShaderType::FRAGMENT, "uViewPos", sUniformType::FLOAT, 3},
        {sShaderType::VERTEX, "uProj", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uView", sUniformType::FLOAT, 4, 4},
    };

    sUniforms uniforms = gfxm.createUniforms(shader, uniformDef);
    static const float fov = 80.0f;
    static const float nearp = 0.001f;
    static const float farp = 1000.0f;
    static const float aspect_ratio = 800.0f / 600.0f;

    bool mouse_locked = false;

#pragma pack(push,1)
    struct ShaderData {
        float time = 0.0f;
        vec3 viewPos = {0.0f, 0.0f, 0.0f};

        mat4 proj = perspective(fov, aspect_ratio, nearp, farp);
        mat4 view = identity();
    } shaderData;
#pragma pack(pop)
    if (uniformDef.size() != sizeof(ShaderData)) {
        printf("ERROR: Uniform definition size does not match shader data size\n");
        printf("Uniform definition size: %d\n", uniformDef.size());
        printf("Shader data size: %d\n", sizeof(ShaderData));
        return 1;
    }

    unsigned int i = 0;

    sCamera camera = {};
    camera.pos = {3.0f, 3.0f, 10.0f};

    float speed = 5.0f;
    float sensitivity = 0.001f;

    winm.setMousePosition(win, 400, 300);

    while (!winm.shouldClose(win)) {
        winm.updateWindow(&win);
        gfxm.clear();

        float mousex, mousey;
        winm.getMousePosition(win, &mousex, &mousey);
        float dx = mousex - 400;
        float dy = mousey - 300;
        
        if (winm.getTime(win) < 0.1 || !mouse_locked) {
            dx = 0;
            dy = 0;
        }
        if (mouse_locked)
            winm.setMousePosition(win, 400, 300);
        
        if (winm.isKeyPressed(win, Key::Escape)) {
            mouse_locked = false;
            winm.setCursorMode(win, CursorMode::Normal);
        }

        if (winm.isMouseButtonPressed(win, 0)) {
            mouse_locked = true;
            winm.setCursorMode(win, CursorMode::Disabled);
        }

        // // WASD
        if (winm.isKeyPressed(win, Key::W)) {
            camMove(&camera, camera.forward, speed * win.dt);
        }
        if (winm.isKeyPressed(win, Key::S)) {
            camMove(&camera, camera.back(), speed * win.dt);
        }
        if (winm.isKeyPressed(win, Key::A)) {
            camMove(&camera, camera.left(), speed * win.dt);
        }
        if (winm.isKeyPressed(win, Key::D)) {
            camMove(&camera, camera.right(), speed * win.dt);
        }
        if (winm.isKeyPressed(win, Key::Q)) {
            camMove(&camera, camera.down(), speed * win.dt);
        }
        if (winm.isKeyPressed(win, Key::E)) {
            camMove(&camera, camera.up, speed * win.dt);
        }

        if (dx != 0) {
            camYaw(&camera, dx * sensitivity);
        }
        if (dy != 0) {
            camPitch(&camera, -dy * sensitivity);
        }

        shaderData.time = (float)winm.getTime(win);
        shaderData.view = view(camera);
        shaderData.viewPos = camera.pos;
        gfxm.useShaderProgram(shader);
        gfxm.setUniforms(uniforms, &shaderData);
        gfxm.useTexture(shader, tex, "tex0"); // order matters, must be called after useShaderProgram
        gfxm.drawMesh(mesh);

        gfxm.present();
        winm.swapBuffers(win);
    }

    winm.destroyWindow(win);

    return 0;
}
