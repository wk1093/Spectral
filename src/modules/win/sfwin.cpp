#include "module.h"
#include <SFML/Graphics.hpp>


const sf::Keyboard::Key* keys = new sf::Keyboard::Key[static_cast<unsigned long>(Key::KeyCount)] {
//        GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_I,
//        GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R,
//        GLFW_KEY_S, GLFW_KEY_T, GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X, GLFW_KEY_Y, GLFW_KEY_Z,
//        GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8,
//        GLFW_KEY_9, GLFW_KEY_ESCAPE, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_ALT, GLFW_KEY_LEFT_SUPER,
//        GLFW_KEY_RIGHT_CONTROL, GLFW_KEY_RIGHT_SHIFT, GLFW_KEY_RIGHT_ALT, GLFW_KEY_RIGHT_SUPER, GLFW_KEY_MENU,
//        GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_SEMICOLON, GLFW_KEY_COMMA, GLFW_KEY_PERIOD,
//        GLFW_KEY_APOSTROPHE, GLFW_KEY_SLASH, GLFW_KEY_BACKSLASH, GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_EQUAL, GLFW_KEY_MINUS,
//        GLFW_KEY_SPACE, GLFW_KEY_ENTER, GLFW_KEY_BACKSPACE, GLFW_KEY_TAB, GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN,
//        GLFW_KEY_END, GLFW_KEY_HOME, GLFW_KEY_INSERT, GLFW_KEY_DELETE, GLFW_KEY_KP_ADD, GLFW_KEY_KP_SUBTRACT,
//        GLFW_KEY_KP_MULTIPLY, GLFW_KEY_KP_DIVIDE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN,
//        GLFW_KEY_KP_0, GLFW_KEY_KP_1, GLFW_KEY_KP_2, GLFW_KEY_KP_3, GLFW_KEY_KP_4, GLFW_KEY_KP_5, GLFW_KEY_KP_6,
//        GLFW_KEY_KP_7, GLFW_KEY_KP_8, GLFW_KEY_KP_9, GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5,
//        GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12, GLFW_KEY_F13,
//        GLFW_KEY_F14, GLFW_KEY_F15, GLFW_KEY_PAUSE
        sf::Keyboard::A, sf::Keyboard::B, sf::Keyboard::C, sf::Keyboard::D, sf::Keyboard::E, sf::Keyboard::F, sf::Keyboard::G, sf::Keyboard::H, sf::Keyboard::I,
        sf::Keyboard::J, sf::Keyboard::K, sf::Keyboard::L, sf::Keyboard::M, sf::Keyboard::N, sf::Keyboard::O, sf::Keyboard::P, sf::Keyboard::Q, sf::Keyboard::R,
        sf::Keyboard::S, sf::Keyboard::T, sf::Keyboard::U, sf::Keyboard::V, sf::Keyboard::W, sf::Keyboard::X, sf::Keyboard::Y, sf::Keyboard::Z,
        sf::Keyboard::Num0, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8,
        sf::Keyboard::Num9, sf::Keyboard::Escape, sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::LAlt, sf::Keyboard::LSystem,
        sf::Keyboard::RControl, sf::Keyboard::RShift, sf::Keyboard::RAlt, sf::Keyboard::RSystem, sf::Keyboard::Menu,
        sf::Keyboard::LBracket, sf::Keyboard::RBracket, sf::Keyboard::SemiColon, sf::Keyboard::Comma, sf::Keyboard::Period,
        sf::Keyboard::Quote, sf::Keyboard::Slash, sf::Keyboard::BackSlash, sf::Keyboard::Tilde, sf::Keyboard::Equal, sf::Keyboard::Dash,
        sf::Keyboard::Space, sf::Keyboard::Return, sf::Keyboard::BackSpace, sf::Keyboard::Tab, sf::Keyboard::PageUp, sf::Keyboard::PageDown,
        sf::Keyboard::End, sf::Keyboard::Home, sf::Keyboard::Insert, sf::Keyboard::Delete, sf::Keyboard::Add, sf::Keyboard::Subtract,
        sf::Keyboard::Multiply, sf::Keyboard::Divide, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down,
        sf::Keyboard::Numpad0, sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Keyboard::Numpad3, sf::Keyboard::Numpad4, sf::Keyboard::Numpad5, sf::Keyboard::Numpad6,
        sf::Keyboard::Numpad7, sf::Keyboard::Numpad8, sf::Keyboard::Numpad9, sf::Keyboard::F1, sf::Keyboard::F2, sf::Keyboard::F3, sf::Keyboard::F4, sf::Keyboard::F5,
        sf::Keyboard::F6, sf::Keyboard::F7, sf::Keyboard::F8, sf::Keyboard::F9, sf::Keyboard::F10, sf::Keyboard::F11, sf::Keyboard::F12, sf::Keyboard::F13,
        sf::Keyboard::F14, sf::Keyboard::F15, sf::Keyboard::Pause
};

sf::Keyboard::Key toSFKey(Key k) {
    return keys[static_cast<unsigned long>(k)];
}

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
#if defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD) || defined(SFML_SYSTEM_OPENBSD) || defined(SFML_SYSTEM_NETBSD)
    auto* ptr = new unsigned long;
    *ptr = (unsigned long)handle;
    return ptr;
#else
    return (void*)handle;
#endif
}

CEXPORT bool isKeyPressed(sWindow window, Key key) {
    return sf::Keyboard::isKeyPressed(toSFKey(key));
}

CEXPORT bool isMouseButtonPressed(sWindow window, int button) {
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

