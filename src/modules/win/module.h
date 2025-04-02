#pragma once
#include "../moduleLib.h"

#include <stdio.h>
#include <chrono>

struct WindowModule;

/*
 * @brief Window module interface.
 * 
 * Window flags structure. This is used to set various properties of the window.

 * @note The flags are not guaranteed to be supported on all platforms or windowing libraries.
 * @note The flags may be ignored or have no effect depending on the windowing library used.
*/
struct sWindowFlags {
    /// @brief Whether vertical sync is enabled.
    /// @details If true, the window will synchronize its frame rate with the monitor's refresh rate.
    /// @note This may help reduce screen tearing and improve performance.
    /// @note This may not be supported on all platforms or windowing libraries.
    bool vsync;
    /// @brief Whether the window is fullscreen.
    /// @details If true, the window will be displayed in fullscreen mode.
    /// @note This may not be supported on all platforms or windowing libraries.
    bool resizable;
};

/**
 * @brief Window structure.
 * 
 * This structure represents a window created by the window module.
 * It contains various properties and methods for interacting with the window.
 * 
 * @note Certain members of this structure are meant to be not accessed directly, but cannot be made private due to the internal implementation.
 */
struct sWindow {
    /// @brief Delta time since the last frame.
    /// @details This is the time in seconds since the last frame was rendered.
    double dt;
    /// @brief The width of the window.
    /// @details This is the width of the window in pixels.
    int width;
    /// @brief The height of the window.
    /// @details This is the height of the window in pixels.
    int height;
    /// @brief The window flags.
    /// @details This is a structure containing various properties of the window.
    /// @see sWindowFlags
    sWindowFlags flags;

    void* internal;
    WindowModule* creator;
    double lastTime;
    std::chrono::high_resolution_clock::time_point startTime;
    bool did_resize;
};

// Required for compatibility with different windowing libraries
enum class Key {
    A=0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Escape, LControl, LShift, LAlt, LSystem, RControl, RShift, RAlt, RSystem,
    Menu, LBracket, RBracket, SemiColon, Comma, Period, Quote, Slash, BackSlash,
    Tilde, Equal, Dash, Space, Return, BackSpace, Tab, PageUp, PageDown, End, Home,
    Insert, Delete, Add, Subtract, Multiply, Divide, Left, Right, Up, Down, Numpad0,
    Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8,
    Numpad9, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11,F12, F13, F14, F15,
    Pause, KeyCount
};
/**
 * @brief Cursor mode enumeration.
 * 
 * This enumeration defines the different modes for the cursor in the window.
 * 
 * @note The cursor mode may not be supported on all platforms or windowing libraries.
 * @see WindowModule::setCursorMode
 */
enum class CursorMode {
    /// @brief Normal cursor mode.
    /// @details The cursor is visible and can be moved freely within the window.
    /// @note This is the default mode.
    Normal = 0,
    /// @brief Hidden cursor mode.
    /// @details The cursor is hidden, might be movable depending on the windowing library.
    /// @warning This may not be supported on all platforms or windowing libraries.
    Hidden,
    /// @brief Locked cursor mode.
    /// @details The cursor is hidden and locked to the window, preventing it from leaving the window area.
    /// @warning This may not be supported on all platforms or windowing libraries.
    /// @note This is useful for first-person camera controls or similar applications.
    Disabled
};

namespace window {
    typedef sWindow* (*WindowLoader)(const char *name, int width, int height, sWindowFlags flags);
    typedef void (*WindowDestructor)(sWindow* window);
    typedef void (*WindowUpdate)(sWindow* window);
    typedef void (*WindowSwapBuffers)(sWindow window);
    typedef bool (*WindowShouldClose)(sWindow window);
    typedef void (*WindowSetShouldClose)(sWindow window, bool value);
    typedef void* (*WindowGetHandle)(sWindow window);
    typedef bool (*WindowIsKeyPressed)(sWindow window, Key key);
    typedef bool (*WindowIsMouseButtonPressed)(sWindow window, int button);
    typedef void (*WindowGetMousePosition)(sWindow window, float* x, float* y);
    typedef void (*WindowSetMousePosition)(sWindow window, float x, float y);
    typedef void (*WindowSetCursorMode)(sWindow window, CursorMode mode);
    typedef void (*WindowSetWindowTitle)(sWindow window, const char* title);
    typedef void (*WindowSetResizable)(sWindow window, bool resizable);
}

/**
 * @brief Window module class.
 * 
 * This class represents the window module and provides methods for creating and managing windows.
 * It is responsible for loading the windowing library and providing access to its functions.
 * 
 * @note This class contains function pointers that will be loaded from the module file (a dynamic library).
 * @note These functions will do something different depending on the implementation selected at runtime.
 */
struct WindowModule : Module {
    window::WindowLoader internal_loadWindow;
    window::WindowUpdate internal_updateWindow;

    /// @brief Destructor function pointer for the window module.
    /// @details This function is used to destroy a window created by the window module.
    /// @note This function will be loaded from the module file (a dynamic library), and the implementation may vary.
    window::WindowDestructor destroyWindow;
    window::WindowSwapBuffers swapBuffers;
    window::WindowShouldClose shouldClose;
    window::WindowSetShouldClose setShouldClose;
    window::WindowGetHandle getHandle;
    window::WindowIsKeyPressed isKeyPressed;
    window::WindowIsMouseButtonPressed isMouseButtonPressed;
    window::WindowGetMousePosition getMousePosition;
    window::WindowSetMousePosition setMousePosition;
    window::WindowSetCursorMode setCursorMode;
    window::WindowSetWindowTitle setWindowTitle;

    sWindow* loadWindow(const char* name, int width, int height, sWindowFlags flags) {
        sWindow* w = internal_loadWindow(name, width, height, flags);
        w->creator = this;
        w->startTime = std::chrono::high_resolution_clock::now();
        w->flags = flags;
        w->width = width;
        w->height = height;
        w->did_resize = false;
        return w;
    }

    sWindow* loadWindow(const char* name, int width, int height, bool vsync=true, bool resizable=false) {
        sWindowFlags flags = {vsync, false};
        return loadWindow(name, width, height, flags);
    }

    void updateWindow(sWindow* window) {
        internal_updateWindow(window);
        double cur = getTime(*window);
        window->dt = cur - window->lastTime;
        window->lastTime = cur;
    }

    double getTime(sWindow window) {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - window.startTime).count();
    }

    explicit WindowModule(const char* dynlib) : Module(dynlib, "win") {
        internal_loadWindow = (window::WindowLoader)lib.getSymbol("loadWindow");
        destroyWindow = (window::WindowDestructor)lib.getSymbol("destroyWindow");
        internal_updateWindow = (window::WindowUpdate)lib.getSymbol("updateWindow");
        swapBuffers = (window::WindowSwapBuffers)lib.getSymbol("swapBuffers");
        shouldClose = (window::WindowShouldClose)lib.getSymbol("shouldClose");
        setShouldClose = (window::WindowSetShouldClose)lib.getSymbol("setShouldClose");
        getHandle = (window::WindowGetHandle)lib.getSymbol("getHandle");
        isKeyPressed = (window::WindowIsKeyPressed)lib.getSymbol("isKeyPressed");
        isMouseButtonPressed = (window::WindowIsMouseButtonPressed)lib.getSymbol("isMouseButtonPressed");
        getMousePosition = (window::WindowGetMousePosition)lib.getSymbol("getMousePosition");
        setMousePosition = (window::WindowSetMousePosition)lib.getSymbol("setMousePosition");
        setCursorMode = (window::WindowSetCursorMode)lib.getSymbol("setCursorMode");
        setWindowTitle = (window::WindowSetWindowTitle)lib.getSymbol("setWindowTitle");
    }

};