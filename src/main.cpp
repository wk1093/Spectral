#include "modules/win/module.h"
#include "modules/gfx/module.h"
#include "modules/shdr/module.h"
#include "modules/math/module.h" // doesn't need to be loaded, because all the functions are inlined and the module is header-only
#include "modules/tex/module.h"
#include "modules/wrld/module.h"
#include "modules/text/module.h"
#define IUI_IMPLEMENTATION
#include "modules/iui/module.h"

#include "cube.h"

#include <cmath>
#include <vector>

// TODO: Make a sample game to test the engine
// Make a basic 3D player controller with gravity and collision
// and make a basic platformer/obby/parkour game
// maybe also make a 3D shooter game
// make all these using a new higher-level API that abstracts the modules and handles everything for you
// make a player controller library with a lot of settings and options (so this will be easier to use than Unity)
// also make a UI library
// also make a basic file format (or use an existing one) to store world/level data and make an editor for it (like Unity but simpler)
// this engine will not be structured like Unity, most things will be in code, unlike Unity where most things are in the editor
// the editor will be used to create levels and worlds, and to set up models, textures, and materials

// TODO: Support for more advanced forms of rendering like instancing, compute shaders, and ray tracing
// TODO: Support for more advanced forms of input like gamepads, touchscreens, and VR
// TODO: Audio
// TODO: Networking
// TODO: Physics
// TODO: AI
// TODO: Scripting
// TODO: ECS
// TODO: Multi-threading

struct Cube {
    sMesh mesh;
    sModelTransform transform;

    Cube(GraphicsModule* gfxm, sShader shader) {
        mesh = gfxm->createMesh(shader, vertices, sizeof(vertices), indices, sizeof(indices));
    }

    Cube(GraphicsModule* gfxm, sShader shader, vec3 pos) {
        mesh = gfxm->createMesh(shader, vertices, sizeof(vertices), indices, sizeof(indices));
        transform.pos = pos;
    }

    void draw(GraphicsModule* gfxm) {
        gfxm->drawMesh(mesh);
    }
};


static const Clay_Color COLOR_ORANGE    = (Clay_Color) {225, 138, 50, 255};
static const Clay_Color COLOR_BLUE      = (Clay_Color) {111, 173, 162, 255};
static const Clay_Color COLOR_LIGHT     = (Clay_Color) {224, 215, 210, 255};


static void Label(const Clay_String text, const int cornerRadius)
{
    CLAY(CLAY_LAYOUT({ .padding = {8, 8} }),
        CLAY_RECTANGLE({
            .color = Clay_Hovered() ? COLOR_BLUE : COLOR_ORANGE,
            .cornerRadius = cornerRadius,
        })) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
           .textColor = { 255, 255, 255, 255 },
           .fontId = 0,
           .fontSize = 30,
        }));
   }
}

static Clay_RenderCommandArray createLayout()
{
    Clay_BeginLayout();
    CLAY(CLAY_ID("MainContent"),
        CLAY_LAYOUT({
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
            },
            .padding = { 10, 10 },
            .childGap = 10,
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER,
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }),
        CLAY_BORDER({
            .left = { 20, COLOR_BLUE },
            .right = { 20, COLOR_BLUE },
            .top = { 20, COLOR_BLUE },
            .bottom = { 20, COLOR_BLUE },
        }),
        CLAY_RECTANGLE({
            .color = COLOR_LIGHT,
        })
    ) {
        Label(CLAY_STRING("Rounded - Button 1"), 5);
        Label(CLAY_STRING("Straight - Button 2") , 0);
        Label(CLAY_STRING("Straight - 3") , 0);
        Label(CLAY_STRING("Rounded+ - Button 4") , 10);
        Label(CLAY_STRING("More test text"), 0);
        Label(CLAY_STRING("What is wrong with text"), 0);
        Label(CLAY_STRING("I need longer text"), 0);
    }
    return Clay_EndLayout();
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
    TextureModule texm("stb");
    if (!texm.lib.valid()) return 1;
    TextModule textm("ft");
    if (!textm.lib.valid()) return 1;

    std::string window_title = "Test (win_" + std::string(window_module) + ", gfx_" + std::string(graphics_module) + ")";

    sWindow win = winm.loadWindow(window_title.c_str(), 800, 600, true);
    gfxm.init(&win);
    textm.init(&gfxm, &shdr);

    gfxm.setClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    sTextureDefinition texDef = texm.loadTexture("textures/test.png");
    sTexture tex = gfxm.createTexture(texDef);
    texm.freeTexture(texDef);

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    sFont font = textm.loadFont("fonts/arial.ttf", 64, "spsl/text.spslv", "spsl/text.spslf");
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    printf("Time to load font: %f\n", elapsed.count());

    // sText textobj = textm.createText(font, "Hello, World!"); // creates a vertex defintion, a shader, and uniforms, and a mesh
    Clay_Spectral_Init(&winm, &gfxm, &textm, &shdr, &win, &font);



    sVertexDefinition* vertDef = gfxm.createVertexDefinition({3, 3, 2});
    if (vertexDefinitionSize(vertDef) != sizeof(Vertex)) {
        printf("ERROR: Vertex definition size does not match vertex size\n");
        return 1;
    }
    
    sShader vert{};
    sShader frag{};
    vert = shdr.compile(&gfxm, "spsl/basic.spslv", sShaderType::VERTEX, vertDef);
    frag = shdr.compile(&gfxm, "spsl/basic.spslf", sShaderType::FRAGMENT);
    sShaderProgram shader = gfxm.createShaderProgram({vert, frag});
    static const float fov = 80.0f;
    static const float nearp = 0.001f;
    static const float farp = 1000.0f;
    static const float aspect_ratio = (float)win.width / (float)win.height;

    bool mouse_locked = false;

    sUniformDefinition uniformDef = {
        {sShaderType::FRAGMENT, "uTime", sUniformType::FLOAT, 1},
        {sShaderType::FRAGMENT, "uViewPos", sUniformType::FLOAT, 3},
        {sShaderType::VERTEX, "uProj", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uView", sUniformType::FLOAT, 4, 4},
        {sShaderType::VERTEX, "uModel", sUniformType::FLOAT, 4, 4}
    };
    struct ShaderData {
        float time = 0.0f;
        vec3 viewPos = {0.0f, 0.0f, 0.0f};
        mat4 proj = perspective(fov, aspect_ratio, nearp, farp);
        mat4 view = identity();
        mat4 model = identity();
    } shaderData;
    if (uniformDef.size() != sizeof(ShaderData)) {
        printf("ERROR: Uniform definition size does not match shader data size\n");
        return 1;
    }
    sUniforms uniforms = gfxm.createUniforms(shader, uniformDef);

    sCamera camera = {};
    camera.pos = {3.0f, 3.0f, 10.0f};

    float speed = 5.0f;
    float sensitivity = 0.001f;

    std::vector<Cube> cubes;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cubes.push_back(Cube(&gfxm, vert, (vec3){(float)(i * 2), 0.0f, (float)(j * 2)}));
        }
    }

    float yvel = 0.0f;

    double lastFPS = 0.0;
    mat4 proj = orthographic(0, win.width, 0, win.height, -1, 1);
    // textm.setTextProj(textobj, proj);
    // textm.setTextModel(textobj, translate({400, 300, 0}));

    while (!winm.shouldClose(win)) {
        winm.updateWindow(&win);
        gfxm.clear();

        double fps = 1.0 / win.dt;
        if (lastFPS < 30.0) lastFPS = fps;
        else lastFPS = lastFPS * 0.95 + fps * 0.05;
        winm.setWindowTitle(win, (window_title + " - " + std::to_string((int)lastFPS) + " FPS").c_str());

        // float mousex, mousey;
        // winm.getMousePosition(win, &mousex, &mousey);
        // float dx = mousex - win.width / 2;
        // float dy = mousey - win.height / 2;
        
        // if (winm.getTime(win) < 0.1 || !mouse_locked) {
        //     dx = 0;
        //     dy = 0;
        // }
        // if (mouse_locked)
        //     winm.setMousePosition(win, win.width / 2, win.height / 2);
        
        // if (winm.isKeyPressed(win, Key::Escape)) {
        //     mouse_locked = false;
        //     winm.setCursorMode(win, CursorMode::Normal);
        // }

        // if (winm.isMouseButtonPressed(win, 0)) {
        //     mouse_locked = true;
        //     winm.setCursorMode(win, CursorMode::Disabled);
        // }

        // if (dx != 0) {
        //     camYaw(&camera, dx * sensitivity);
        // }
        // if (dy != 0) {
        //     camPitch(&camera, -dy * sensitivity);
        // }

        // vec3 fixed_forward = camera.forward; // we need to align the forward so it doesn't go up and down
        // fixed_forward.y = 0;
        // fixed_forward = normalize(fixed_forward);
        // // basic movement and gravity (no jumping yet)
        // if (winm.isKeyPressed(win, Key::W)) {
        //     camMove(&camera, fixed_forward, speed * win.dt);
        // }
        // if (winm.isKeyPressed(win, Key::S)) {
        //     camMove(&camera, -fixed_forward, speed * win.dt);
        // }
        // if (winm.isKeyPressed(win, Key::A)) {
        //     camMove(&camera, camera.left(fixed_forward), speed * win.dt);
        // }
        // if (winm.isKeyPressed(win, Key::D)) {
        //     camMove(&camera, camera.right(fixed_forward), speed * win.dt);
        // }

        // // gravity
        // if (camera.pos.y > 3) {
        //     yvel -= 9.8f * win.dt;
        // } else {
        //     camera.pos.y = 3;
        //     yvel = 0;
        // }

        // // jumping
        // if (winm.isKeyPressed(win, Key::Space) && camera.pos.y == 3) {
        //     yvel = 250.0f * win.dt;
        // }

        // camera.pos.y += yvel * win.dt;
        // yvel *= 0.99f * (win.dt * 60);


        Clay_RenderCommandArray layout = createLayout();
        Clay_Spectral_Render(&win, layout, proj, identity());


        gfxm.useShaderProgram(shader);
        shaderData.time = (float)winm.getTime(win);
        shaderData.view = view(camera);
        shaderData.viewPos = camera.pos;

        for (Cube& cube : cubes) {
            gfxm.useShaderProgram(shader);
            shaderData.model = cube.transform.matrix();
            gfxm.setUniforms(uniforms, &shaderData);
            gfxm.useTexture(shader, tex, "tex0");
            cube.draw(&gfxm);
        }

        // textm.drawText(textobj);

        

        gfxm.present();
        winm.swapBuffers(win);
    }

    // textm.freeText(textobj);
    textm.freeFont(font);

    gfxm.freeShaderProgram(shader);
    gfxm.freeShader(vert);
    gfxm.freeShader(frag);
    gfxm.freeTexture(tex);
    gfxm.freeUniforms(uniforms);
    gfxm.freeVertexDefinition(vertDef);
    gfxm.destroy();

    winm.destroyWindow(win);

    return 0;
}
