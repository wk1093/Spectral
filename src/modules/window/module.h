#pragma once
#include "../moduleLib.h"

#include <stdio.h>

struct sWindow;

namespace window {
    typedef sWindow (*WindowLoader)(const char *name, int width, int height);
    typedef void (*WindowDestructor)(sWindow window);
    typedef void (*WindowUpdate)(sWindow window);
    typedef void (*WindowSwapBuffers)(sWindow window);
    typedef bool (*WindowShouldClose)(sWindow window);
    typedef void (*WindowSetShouldClose)(sWindow window, bool value);
    typedef void* (*WindowGetHandle)(sWindow window);
    typedef bool (*WindowIsKeyPressed)(sWindow window, int key);
    typedef bool (*WindowIsMouseButtonPressed)(sWindow window, int button);
    typedef void (*WindowGetMousePosition)(sWindow window, float* x, float* y);
    typedef void (*WindowSetMousePosition)(sWindow window, float x, float y);
}

struct sWindow {
    void* internal;
};

struct WindowModule : Module {
    window::WindowLoader loadWindow;
    window::WindowDestructor destroyWindow;
    window::WindowUpdate updateWindow;
    window::WindowSwapBuffers swapBuffers;
    window::WindowShouldClose shouldClose;
    window::WindowSetShouldClose setShouldClose;
    window::WindowGetHandle getHandle;
    window::WindowIsKeyPressed isKeyPressed;
    window::WindowIsMouseButtonPressed isMouseButtonPressed;
    window::WindowGetMousePosition getMousePosition;
    window::WindowSetMousePosition setMousePosition;
    DynamicLibrary lib;

    explicit WindowModule(const char* dynlib) : lib(dynlib) {
        loadWindow = (window::WindowLoader)lib.getSymbol("loadWindow");
        destroyWindow = (window::WindowDestructor)lib.getSymbol("destroyWindow");
        updateWindow = (window::WindowUpdate)lib.getSymbol("updateWindow");
        swapBuffers = (window::WindowSwapBuffers)lib.getSymbol("swapBuffers");
        shouldClose = (window::WindowShouldClose)lib.getSymbol("shouldClose");
        setShouldClose = (window::WindowSetShouldClose)lib.getSymbol("setShouldClose");
        getHandle = (window::WindowGetHandle)lib.getSymbol("getHandle");
        isKeyPressed = (window::WindowIsKeyPressed)lib.getSymbol("isKeyPressed");
        isMouseButtonPressed = (window::WindowIsMouseButtonPressed)lib.getSymbol("isMouseButtonPressed");
        getMousePosition = (window::WindowGetMousePosition)lib.getSymbol("getMousePosition");
        setMousePosition = (window::WindowSetMousePosition)lib.getSymbol("setMousePosition");
    }

};