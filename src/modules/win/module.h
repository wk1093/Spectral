#pragma once
#include "../moduleLib.h"

#include <stdio.h>
#include <chrono>

struct WindowModule;

struct sWindow {
    void* internal;
    WindowModule* creator;
    double dt;
    double lastTime;
    std::chrono::high_resolution_clock::time_point startTime;
    bool vsync;
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

enum class CursorMode {
    Normal = 0,
    Hidden,
    Disabled
};

namespace window {
    typedef sWindow (*WindowLoader)(const char *name, int width, int height, bool vsync);
    typedef void (*WindowDestructor)(sWindow window);
    typedef void (*WindowUpdate)(sWindow window);
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
}

struct WindowModule : Module {
    window::WindowLoader internal_loadWindow;
    window::WindowDestructor destroyWindow;
    window::WindowUpdate internal_updateWindow;
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

    sWindow loadWindow(const char* name, int width, int height, bool vsync) {
        sWindow w = internal_loadWindow(name, width, height, vsync);
        w.creator = this;
        w.startTime = std::chrono::high_resolution_clock::now();
        w.vsync = vsync;
        return w;
    }

    void updateWindow(sWindow* window) {
        internal_updateWindow(*window);
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