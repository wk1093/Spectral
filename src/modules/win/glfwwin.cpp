#include <GLFW/glfw3.h>
#include <glfw_config.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(__linux__)
#ifdef _GLFW_X11
#define GLFW_EXPOSE_NATIVE_X11 // TODO: Wayland
#elif defined(_GLFW_WAYLAND)
#define GLFW_EXPOSE_NATIVE_WAYLAND
#else
#error "No native window handle exposed for this platform"
#endif
#endif

#include <GLFW/glfw3native.h>

#include "module.h"

sArenaAllocator* gArena = nullptr;

CEXPORT size_t getDesiredArenaSize() {
    return sizeof(sWindow);
}

CEXPORT void moduleInit(sArenaAllocator* arena) {
    gArena = arena;
}

const int* keys = new int[static_cast<unsigned long>(Key::KeyCount)] {
        GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_I,
        GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R,
        GLFW_KEY_S, GLFW_KEY_T, GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X, GLFW_KEY_Y, GLFW_KEY_Z,
        GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8,
        GLFW_KEY_9, GLFW_KEY_ESCAPE, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_ALT, GLFW_KEY_LEFT_SUPER,
        GLFW_KEY_RIGHT_CONTROL, GLFW_KEY_RIGHT_SHIFT, GLFW_KEY_RIGHT_ALT, GLFW_KEY_RIGHT_SUPER, GLFW_KEY_MENU,
        GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_SEMICOLON, GLFW_KEY_COMMA, GLFW_KEY_PERIOD,
        GLFW_KEY_APOSTROPHE, GLFW_KEY_SLASH, GLFW_KEY_BACKSLASH, GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_EQUAL, GLFW_KEY_MINUS,
        GLFW_KEY_SPACE, GLFW_KEY_ENTER, GLFW_KEY_BACKSPACE, GLFW_KEY_TAB, GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN,
        GLFW_KEY_END, GLFW_KEY_HOME, GLFW_KEY_INSERT, GLFW_KEY_DELETE, GLFW_KEY_KP_ADD, GLFW_KEY_KP_SUBTRACT,
        GLFW_KEY_KP_MULTIPLY, GLFW_KEY_KP_DIVIDE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN,
        GLFW_KEY_KP_0, GLFW_KEY_KP_1, GLFW_KEY_KP_2, GLFW_KEY_KP_3, GLFW_KEY_KP_4, GLFW_KEY_KP_5, GLFW_KEY_KP_6,
        GLFW_KEY_KP_7, GLFW_KEY_KP_8, GLFW_KEY_KP_9, GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5,
        GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12, GLFW_KEY_F13,
        GLFW_KEY_F14, GLFW_KEY_F15, GLFW_KEY_PAUSE
};

int toGLKey(Key k) {
    return keys[static_cast<unsigned long>(k)];
}

// viewport size callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    sWindow* win = (sWindow*)glfwGetWindowUserPointer(window);
    win->width = width;
    win->height = height;
    win->did_resize = true;
}

CEXPORT sWindow* loadWindow(const char* title, int width, int height, sWindowFlags flags) {
    if (!glfwInit()) {
        printf("Error initializing GLFW\n");
        return {nullptr};
    }
#ifdef SPECTRAL_GLFW_OPENGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#elif defined(SPECTRAL_GLFW_NOAPI)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, flags.resizable ? GLFW_TRUE : GLFW_FALSE);
    GLFWwindow* internal = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(internal);
    if (flags.vsync)
        glfwSwapInterval(1);
    if (!internal) {
        glfwTerminate();
        printf("Error creating window\n");
        return {nullptr};
    }

    sWindow* win = gArena->allocate<sWindow>();
    win->internal = internal;
    win->width = width;
    win->height = height;
    win->flags = flags;

    // set viewport size callback
    glfwSetWindowUserPointer(internal, win);
    glfwSetFramebufferSizeCallback(internal, framebuffer_size_callback);

    return win;
}

CEXPORT void destroyWindow(sWindow* window) {
    glfwDestroyWindow((GLFWwindow*)(window->internal));
    glfwTerminate();
}

CEXPORT void updateWindow(sWindow* window) {
    glfwPollEvents();
}

CEXPORT void swapBuffers(sWindow window) {
    glfwSwapBuffers((GLFWwindow*)window.internal);
}

CEXPORT bool shouldClose(sWindow window) {
    return glfwWindowShouldClose((GLFWwindow*)window.internal);
}

CEXPORT void setShouldClose(sWindow window, bool value) {
    glfwSetWindowShouldClose((GLFWwindow*)window.internal, value);
}

CEXPORT void* getHandle(sWindow window) {
    // get native window handle
    // TODO: Test other platforms
#ifdef GLFW_EXPOSE_NATIVE_WIN32
    return glfwGetWin32Window((GLFWwindow*)window.internal);
#elif defined(GLFW_EXPOSE_NATIVE_COCOA)
    return glfwGetCocoaWindow((GLFWwindow*)window.internal);
#elif defined(GLFW_EXPOSE_NATIVE_X11)
    auto handle = glfwGetX11Window((GLFWwindow*)window.internal);
    printf("X11 window handle: %lu\n", handle);
    auto* ptr = new unsigned long;
    *ptr = handle;
    return ptr;
#elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
    return glfwGetWaylandWindow((GLFWwindow*)window.internal);
#else
#error "No native window handle exposed for this platform"
#endif
}

CEXPORT bool isKeyPressed(sWindow window, Key key) {
    return glfwGetKey((GLFWwindow*)window.internal, toGLKey(key)) == GLFW_PRESS;
}

CEXPORT bool isMouseButtonPressed(sWindow window, int button) {
    return glfwGetMouseButton((GLFWwindow*)window.internal, button) == GLFW_PRESS;
}

CEXPORT void getMousePosition(sWindow window, float* x, float* y) {
    double xpos, ypos;
    glfwGetCursorPos((GLFWwindow*)window.internal, &xpos, &ypos);
    *x = (float)xpos;
    *y = (float)ypos;
}

CEXPORT void setMousePosition(sWindow window, float x, float y) {
    glfwSetCursorPos((GLFWwindow*)window.internal, x, y);
}

CEXPORT void setCursorMode(sWindow window, CursorMode mode) {
    switch (mode) {
        case CursorMode::Normal:
            glfwSetInputMode((GLFWwindow*)window.internal, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CursorMode::Hidden:
            glfwSetInputMode((GLFWwindow*)window.internal, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case CursorMode::Disabled:
            glfwSetInputMode((GLFWwindow*)window.internal, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
    }
}

CEXPORT void setWindowTitle(sWindow window, const char* title) {
    glfwSetWindowTitle((GLFWwindow*)window.internal, title);
}