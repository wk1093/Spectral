/**
 * @file win/module.h
 * @brief Window module interface.
 */

#pragma once
#include "moduleLib.h"

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

typedef struct sWindowFlags sWindowFlags_t;

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
    /// @see #sWindowFlags_t
    sWindowFlags flags;

    void* internal;
    WindowModule* creator;
    double lastTime;
    std::chrono::high_resolution_clock::time_point startTime;
    bool did_resize;
    const char* origTitle;
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

/// @cond DOXYGEN_OMIT
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
    typedef void (*WindowGetInstanceExtensions)(sWindow window, const char*** extensions, int* count);
}
/// @endcond

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

private:
    window::WindowLoader internal_loadWindow;
    window::WindowUpdate internal_updateWindow;
    window::WindowDestructor internal_destroyWindow;
    window::WindowSwapBuffers internal_swapBuffers;
    window::WindowShouldClose internal_shouldClose;
    window::WindowSetShouldClose internal_setShouldClose;
    window::WindowGetHandle internal_getHandle;
    window::WindowIsKeyPressed internal_isKeyPressed;
    window::WindowIsMouseButtonPressed internal_isMouseButtonPressed;
    window::WindowGetMousePosition internal_getMousePosition;
    window::WindowSetMousePosition internal_setMousePosition;
    window::WindowSetCursorMode internal_setCursorMode;
    window::WindowSetWindowTitle internal_setWindowTitle;
    window::WindowGetInstanceExtensions internal_getWindowInstanceExtensions;

public:

    /**
     * @brief Load a window with the specified name, width, height, and flags.
     *
     * @details This function creates a new window with the specified properties and returns a pointer to it.
     * 
     * @param name The name of the window.
     * @param width The width of the window in pixels.
     * @param height The height of the window in pixels.
     * @param flags The window flags (e.g., vsync, resizable).
     * @return A pointer to the created window.
     */
    inline sWindow* loadWindow(const char* name, int width, int height, sWindowFlags flags) {
        sWindow* w = internal_loadWindow(name, width, height, flags);
        w->creator = this;
        w->startTime = std::chrono::high_resolution_clock::now();
        w->flags = flags;
        w->width = width;
        w->height = height;
        w->did_resize = false;
        w->origTitle = name;
        return w;
    }

    /**
     * @brief Load a window with the specified name, width, height, and default flags.
     * 
     * @details This function creates a new window with the specified properties and default flags (vsync=true, resizable=false).
     * 
     * @param name The name of the window.
     * @param width The width of the window in pixels.
     * @param height The height of the window in pixels.
     * @param vsync Whether vertical sync is enabled (default: true).
     * @param resizable Whether the window is resizable (default: false).
     * @return A pointer to the created window.
     */
    inline sWindow* loadWindow(const char* name, int width, int height, bool vsync=true, bool resizable=false) {
        sWindowFlags flags = {vsync, false};
        return loadWindow(name, width, height, flags);
    }

    /**
     * @brief Update the window properties and state.
     * 
     * @details This function updates the window properties and state, including the delta time since the last frame.
     * 
     * @param window A pointer to the window to update.
     * @note This function should be called once per frame to ensure the window is updated correctly.
     */
    inline void updateWindow(sWindow* window) {
        internal_updateWindow(window);
        double cur = getTime(*window);
        window->dt = cur - window->lastTime;
        window->lastTime = cur;
    }

    /**
     * @brief Destroy the specified window and free its resources.
     * 
     * @details This function destroys the specified window and frees any resources associated with it.
     * 
     * @param window A pointer to the window to destroy.
     * @note This function should be called when the window is no longer needed to free up resources.
     */
    inline void destroyWindow(sWindow* window) {
        internal_destroyWindow(window);
    }

    /**
     * @brief Swap the front and back buffers of the specified window.
     * 
     * @details This function swaps the front and back buffers of the specified window, displaying the rendered content.
     * 
     * @param window A pointer to the window to swap buffers for.
     * @note This function should be called after rendering to the window to display the content.
     */
    inline void swapBuffers(sWindow window) {
        internal_swapBuffers(window);
    }

    /**
     * @brief Check if the specified window should close.
     * 
     * @details This function checks if the specified window should close based on its internal state.
     * 
     * @param window A pointer to the window to check.
     * @return true if the window should close, false otherwise.
     * @note This function is typically used to determine if the window should be closed based on user input or other conditions.
     */
    inline bool shouldClose(sWindow window) {
        return internal_shouldClose(window);
    }

    /**
     * @brief Set the specified window to close or not.
     * 
     * @details This function sets the specified window to close or not based on the provided value.
     * 
     * @param window A pointer to the window to set.
     * @param value true to set the window to close, false to keep it open.
     * @note This function is typically used to programmatically close the window based on user input or other conditions.
     * @note This function may not be supported on all platforms or windowing libraries.
     */
    inline void setShouldClose(sWindow window, bool value) {
        internal_setShouldClose(window, value);
    }

    /**
     * @brief Get the handle of the specified window.
     * 
     * @details This function retrieves the handle of the specified window, which can be used for platform-specific operations.
     * 
     * @param window A pointer to the window to get the handle for.
     * @return A pointer to the handle of the window.
     * @note Unlike the other functions, this one MUST be supported by all windowing libraries for the graphics module to work.
     */
    inline void* getHandle(sWindow window) {
        return internal_getHandle(window);
    }

    /**
     * @brief Check if a specific key is pressed in the specified window.
     * 
     * @details This function checks if a specific key is currently pressed in the specified window.
     * 
     * @param window A pointer to the window to check.
     * @param key The key to check for (e.g., Key::A, Key::Escape).
     * @return true if the key is pressed, false otherwise.
     * @note This should not be relied on for gameplay, as there will be an input module for that
     * @todo Update this once the input module is implemented.
     */
    inline bool isKeyPressed(sWindow window, Key key) {
        return internal_isKeyPressed(window, key);
    }

    /**
     * @brief Check if a specific mouse button is pressed in the specified window.
     * 
     * @details This function checks if a specific mouse button is currently pressed in the specified window.
     * 
     * @param window A pointer to the window to check.
     * @param button The mouse button to check for (e.g., 0 for left button, 1 for right button).
     * @return true if the mouse button is pressed, false otherwise.
     * @note This should not be relied on for gameplay, as there will be an input module for that
     * @todo Update this once the input module is implemented.
     */
    inline bool isMouseButtonPressed(sWindow window, int button) {
        return internal_isMouseButtonPressed(window, button);
    }

    /**
     * @brief Get the current mouse position in the specified window.
     * 
     * @details This function retrieves the current mouse position in the specified window.
     * 
     * @param window A pointer to the window to get the mouse position for.
     * @param x A pointer to store the x-coordinate of the mouse position.
     * @param y A pointer to store the y-coordinate of the mouse position.
     * @note The coordinates might be relative to the window or the screen, depending on the windowing library, this should only be used for calculating deltas.
     * @note This should not be relied on for gameplay, as there will be an input module for that
     * @todo Update this once the input module is implemented.
     */
    inline void getMousePosition(sWindow window, float* x, float* y) {
        internal_getMousePosition(window, x, y);
    }

    /**
     * @brief Set the mouse position in the specified window.
     * 
     * @details This function sets the mouse position in the specified window.
     * 
     * @param window A pointer to the window to set the mouse position for.
     * @param x The x-coordinate of the mouse position.
     * @param y The y-coordinate of the mouse position.
     * @note This might not be supported on all platforms or windowing libraries.
     * @note This should not be relied on for gameplay, as there will be an input module for that
     * @todo Update this once the input module is implemented.
     */
    inline void setMousePosition(sWindow window, float x, float y) {
        internal_setMousePosition(window, x, y);
    }

    /**
     * @brief Set the cursor mode for the specified window.
     * 
     * @details This function sets the cursor mode for the specified window.
     * 
     * @param window A pointer to the window to set the cursor mode for.
     * @param mode The cursor mode to set (e.g., CursorMode::Normal, CursorMode::Hidden).
     * @note This may not be supported on all platforms or windowing libraries.
     * @note This is useful for first-person camera controls or similar applications.
     * @note This should not be relied on for gameplay, as there will be an input module for that
     * @todo Update this once the input module is implemented.
     * @see #CursorMode
     */
    inline void setCursorMode(sWindow window, CursorMode mode) {
        internal_setCursorMode(window, mode);
    }

    /**
     * @brief Set the title of the specified window.
     * 
     * @details This function sets the title of the specified window.
     * 
     * @param window A pointer to the window to set the title for.
     * @param title The title to set for the window.
     * @note This may not be supported on all platforms or windowing libraries.
     * @note This is useful for displaying the current state of the application or game in the window title.
     */
    inline void setWindowTitle(sWindow window, const char* title) {
        internal_setWindowTitle(window, title);
    }

    /**
     * @brief Get the time since the window was created.
     * 
     * @details This function retrieves the time since the window was created in seconds.
     * 
     * @param window A pointer to the window to get the time for.
     * @return The time since the window was created in seconds.
     * @note This can be used for timing purposes, such as calculating the frame rate or elapsed time.
     */
    inline double getTime(sWindow window) {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - window.startTime).count();
    }

    /**
     * @brief Get the instance extensions for the specified window.
     * 
     * @details This function retrieves the instance extensions for the specified window.
     * 
     * @param window A pointer to the window to get the instance extensions for.
     * @param extensions A pointer to store the instance extensions.
     * @param count A pointer to store the number of instance extensions.
     * @note This may not be supported on all platforms or windowing libraries.
     * @note This is mainly used for Vulkan.
     */
    inline void getInstanceExtensions(sWindow window, const char*** extensions, int* count) {
        internal_getWindowInstanceExtensions(window, extensions, count);
    }

    /**
     * @brief Constructor for the WindowModule class.
     * 
     * @details This constructor loads the windowing library and initializes the function pointers.
     * 
     * @param dynlib The name of the dynamic library to load.
     * @note This constructor should be called before using any of the window module functions.
     */
    inline explicit WindowModule(const char* dynlib) : Module(dynlib, "win") {
        internal_loadWindow = (window::WindowLoader)lib.getSymbol("loadWindow");
        internal_destroyWindow = (window::WindowDestructor)lib.getSymbol("destroyWindow");
        internal_updateWindow = (window::WindowUpdate)lib.getSymbol("updateWindow");
        internal_swapBuffers = (window::WindowSwapBuffers)lib.getSymbol("swapBuffers");
        internal_shouldClose = (window::WindowShouldClose)lib.getSymbol("shouldClose");
        internal_setShouldClose = (window::WindowSetShouldClose)lib.getSymbol("setShouldClose");
        internal_getHandle = (window::WindowGetHandle)lib.getSymbol("getHandle");
        internal_isKeyPressed = (window::WindowIsKeyPressed)lib.getSymbol("isKeyPressed");
        internal_isMouseButtonPressed = (window::WindowIsMouseButtonPressed)lib.getSymbol("isMouseButtonPressed");
        internal_getMousePosition = (window::WindowGetMousePosition)lib.getSymbol("getMousePosition");
        internal_setMousePosition = (window::WindowSetMousePosition)lib.getSymbol("setMousePosition");
        internal_setCursorMode = (window::WindowSetCursorMode)lib.getSymbol("setCursorMode");
        internal_setWindowTitle = (window::WindowSetWindowTitle)lib.getSymbol("setWindowTitle");
        internal_getWindowInstanceExtensions = (window::WindowGetInstanceExtensions)lib.getSymbol("getInstanceExtensions");
    }
};