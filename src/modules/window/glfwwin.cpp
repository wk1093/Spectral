
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11 // TODO: Wayland
#endif

#include <GLFW/glfw3native.h>

#include "module.h"

CEXPORT sWindow loadWindow(const char* title, int width, int height) {
    if (!glfwInit()) {
        printf("Error initializing GLFW\n");
        return {nullptr};
    }
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* internal = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(internal);
    glfwSwapInterval(1);
    if (!internal) {
        glfwTerminate();
        printf("Error creating window\n");
        return {nullptr};
    }
    return {internal};

}

CEXPORT void destroyWindow(sWindow window) {
    glfwDestroyWindow((GLFWwindow*)window.internal);
}

CEXPORT void updateWindow(sWindow window) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

CEXPORT void swapBuffers(sWindow window) {
    glfwSwapBuffers((GLFWwindow*)window.internal);
    glfwPollEvents();
}

CEXPORT bool shouldClose(sWindow window) {
    return glfwWindowShouldClose((GLFWwindow*)window.internal);
}

CEXPORT void setShouldClose(sWindow window, bool value) {
    glfwSetWindowShouldClose((GLFWwindow*)window.internal, value);
}

CEXPORT void* getHandle(sWindow window) {
    // get native window handle
#ifdef GLFW_EXPOSE_NATIVE_WIN32
    return glfwGetWin32Window((GLFWwindow*)window.internal);
#elif defined(GLFW_EXPOSE_NATIVE_COCOA)
    return glfwGetCocoaWindow((GLFWwindow*)window.internal);
#elif defined(GLFW_EXPOSE_NATIVE_X11)
    return (void*)glfwGetX11Window((GLFWwindow*)window.internal);
#elif defined(GLFW_EXPOSE_NATIVE_WAYLAND)
    return glfwGetWaylandWindow((GLFWwindow*)window.internal);
#else
#error "No native window handle exposed for this platform"
#endif
}

CEXPORT bool isKeyPressed(sWindow window, int key) {
    return glfwGetKey((GLFWwindow*)window.internal, key) == GLFW_PRESS;
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





