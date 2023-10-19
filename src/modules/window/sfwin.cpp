#include "module.h"
#include <SFML/Graphics.hpp>

CEXPORT sWindow loadWindow(const char* title, int width, int height) {
    auto* win = new sf::RenderWindow(sf::VideoMode(width, height), title);
    if (!win->isOpen()) {
        printf("Error creating window\n");
        return {nullptr};
    }

    return {win};
}

CEXPORT void destroyWindow(sWindow window) {
    ((sf::RenderWindow*)window.internal)->close();
    delete (sf::RenderWindow*)window.internal;
}

CEXPORT void updateWindow(sWindow window) {
    sf::Event event;
    while (((sf::RenderWindow*)window.internal)->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            ((sf::RenderWindow*)window.internal)->close();
        }
    }
    ((sf::RenderWindow*)window.internal)->clear(sf::Color::Black);
}

CEXPORT void swapBuffers(sWindow window) {
    ((sf::RenderWindow*)window.internal)->display();
}

CEXPORT bool shouldClose(sWindow window) {
    return !((sf::RenderWindow*)window.internal)->isOpen();
}

CEXPORT void setShouldClose(sWindow window, bool value) {
    if (value) {
        ((sf::RenderWindow*)window.internal)->close();
    }
}

CEXPORT void* getHandle(sWindow window) {
    sf::WindowHandle handle = ((sf::RenderWindow*)window.internal)->getSystemHandle();
    auto* ptr = new sf::WindowHandle;
    *ptr = handle;
    printf("handle: %lu\n", handle);
    return ptr;
}

CEXPORT bool isKeyPressed(int key) {
    return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)key);
}

CEXPORT bool isMouseButtonPressed(int button) {
    return sf::Mouse::isButtonPressed((sf::Mouse::Button)button);
}

CEXPORT void setMousePosition(sWindow window, int x, int y) {
    sf::Mouse::setPosition(sf::Vector2i(x, y));
}

CEXPORT void getMousePosition(sWindow window, int* x, int* y) {
    sf::Vector2i pos = sf::Mouse::getPosition();
    *x = pos.x;
    *y = pos.y;
}

