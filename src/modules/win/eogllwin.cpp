
#include <eogll.h>
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

const int* keys = new int[static_cast<unsigned long>(Key::KeyCount)] {
        EOGLL_KEY_A, EOGLL_KEY_B, EOGLL_KEY_C, EOGLL_KEY_D, EOGLL_KEY_E, EOGLL_KEY_F, EOGLL_KEY_G, EOGLL_KEY_H, EOGLL_KEY_I,
        EOGLL_KEY_J, EOGLL_KEY_K, EOGLL_KEY_L, EOGLL_KEY_M, EOGLL_KEY_N, EOGLL_KEY_O, EOGLL_KEY_P, EOGLL_KEY_Q, EOGLL_KEY_R,
        EOGLL_KEY_S, EOGLL_KEY_T, EOGLL_KEY_U, EOGLL_KEY_V, EOGLL_KEY_W, EOGLL_KEY_X, EOGLL_KEY_Y, EOGLL_KEY_Z,
        EOGLL_KEY_0, EOGLL_KEY_1, EOGLL_KEY_2, EOGLL_KEY_3, EOGLL_KEY_4, EOGLL_KEY_5, EOGLL_KEY_6, EOGLL_KEY_7, EOGLL_KEY_8,
        EOGLL_KEY_9, EOGLL_KEY_ESCAPE, EOGLL_KEY_LEFT_CONTROL, EOGLL_KEY_LEFT_SHIFT, EOGLL_KEY_LEFT_ALT, EOGLL_KEY_LEFT_SUPER,
        EOGLL_KEY_RIGHT_CONTROL, EOGLL_KEY_RIGHT_SHIFT, EOGLL_KEY_RIGHT_ALT, EOGLL_KEY_RIGHT_SUPER, EOGLL_KEY_MENU,
        EOGLL_KEY_LEFT_BRACKET, EOGLL_KEY_RIGHT_BRACKET, EOGLL_KEY_SEMICOLON, EOGLL_KEY_COMMA, EOGLL_KEY_PERIOD,
        EOGLL_KEY_APOSTROPHE, EOGLL_KEY_SLASH, EOGLL_KEY_BACKSLASH, EOGLL_KEY_GRAVE_ACCENT, EOGLL_KEY_EQUAL, EOGLL_KEY_MINUS,
        EOGLL_KEY_SPACE, EOGLL_KEY_ENTER, EOGLL_KEY_BACKSPACE, EOGLL_KEY_TAB, EOGLL_KEY_PAGE_UP, EOGLL_KEY_PAGE_DOWN,
        EOGLL_KEY_END, EOGLL_KEY_HOME, EOGLL_KEY_INSERT, EOGLL_KEY_DELETE, EOGLL_KEY_KP_ADD, EOGLL_KEY_KP_SUBTRACT,
        EOGLL_KEY_KP_MULTIPLY, EOGLL_KEY_KP_DIVIDE, EOGLL_KEY_LEFT, EOGLL_KEY_RIGHT, EOGLL_KEY_UP, EOGLL_KEY_DOWN,
        EOGLL_KEY_KP_0, EOGLL_KEY_KP_1, EOGLL_KEY_KP_2, EOGLL_KEY_KP_3, EOGLL_KEY_KP_4, EOGLL_KEY_KP_5, EOGLL_KEY_KP_6,
        EOGLL_KEY_KP_7, EOGLL_KEY_KP_8, EOGLL_KEY_KP_9, EOGLL_KEY_F1, EOGLL_KEY_F2, EOGLL_KEY_F3, EOGLL_KEY_F4, EOGLL_KEY_F5,
        EOGLL_KEY_F6, EOGLL_KEY_F7, EOGLL_KEY_F8, EOGLL_KEY_F9, EOGLL_KEY_F10, EOGLL_KEY_F11, EOGLL_KEY_F12, EOGLL_KEY_F13,
        EOGLL_KEY_F14, EOGLL_KEY_F15, EOGLL_KEY_PAUSE
};

int toGLKey(Key k) {
    return keys[static_cast<unsigned long>(k)];
}

CEXPORT sWindow loadWindow(const char* title, int width, int height, bool vsync) {
    if (eogllInit() != EOGLL_SUCCESS) {
        printf("Error initializing EOGLL\n");
        return {nullptr};
    }
    EogllWindow* internal = eogllCreateWindow(width, height, title, eogllDefaultWindowHints());
    if (!internal) {
        eogllTerminate();
        printf("Error creating window\n");
        return {nullptr};
    }
    if (vsync)
        glfwSwapInterval(1);
    return {internal};

}

CEXPORT void destroyWindow(sWindow window) {
    eogllDestroyWindow((EogllWindow*)window.internal);
    eogllTerminate();
}

CEXPORT void updateWindow(sWindow window) {
    eogllPollEvents((EogllWindow*)window.internal);
    // glfwPollEvents();
}

CEXPORT void swapBuffers(sWindow window) {
    eogllSwapBuffers((EogllWindow*)window.internal);
}

CEXPORT bool shouldClose(sWindow window) {
    return eogllWindowShouldClose((EogllWindow*)window.internal);
}

CEXPORT void setShouldClose(sWindow window, bool value) {
    eogllSetWindowShouldClose((EogllWindow*)window.internal, value);
}

CEXPORT void* getHandle(sWindow window) {
    // get native window handle
    auto* win = (EogllWindow*)window.internal;
    GLFWwindow* gwin = win->window;

    // TODO: Test other platforms
#ifdef GLFW_EXPOSE_NATIVE_WIN32
    return glfwGetWin32Window(gwin);
#elif defined(GLFW_EXPOSE_NATIVE_COCOA)
    return glfwGetCocoaWindow(gwin);
#elif defined(GLFW_EXPOSE_NATIVE_X11)
    auto handle = glfwGetX11Window(gwin);
    printf("X11 window handle: %lu\n", handle);
    auto* ptr = new unsigned long;
    *ptr = handle;
    return ptr;
#elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
    return glfwGetWaylandWindow(gwin);
#else
#error "No native window handle exposed for this platform"
#endif
}

CEXPORT bool isKeyPressed(sWindow window, Key key) {
    return ((EogllWindow*)window.internal)->isDown[toGLKey(key)];
}

CEXPORT bool isMouseButtonPressed(sWindow window, int button) {
    return ((EogllWindow*)window.internal)->mouseIsDown[button];
}

CEXPORT void getMousePosition(sWindow window, float* x, float* y) {
    *x = (float)((EogllWindow*)window.internal)->mousex;
    *y = (float)((EogllWindow*)window.internal)->mousey;
}

CEXPORT void setMousePosition(sWindow window, float x, float y) {
    eogllSetCursorPos((EogllWindow*)window.internal, x, y);
}

CEXPORT void setCursorMode(sWindow window, CursorMode mode) {
    switch (mode) {
        case CursorMode::Normal:
            eogllSetCursorMode((EogllWindow*)window.internal, EOGLL_CURSOR_NORMAL);
            break;
        case CursorMode::Hidden:
            eogllSetCursorMode((EogllWindow*)window.internal, EOGLL_CURSOR_HIDDEN);
            break;
        case CursorMode::Disabled:
            eogllSetCursorMode((EogllWindow*)window.internal, EOGLL_CURSOR_DISABLED);
            break;
    }
}

CEXPORT void setWindowTitle(sWindow window, const char* title) {
    glfwSetWindowTitle(((EogllWindow*)window.internal)->window, title);
}