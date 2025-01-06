#include "module.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

sWindow* __sfWindow;

CEXPORT void init(sWindow* win) {
    if (strcmp(win->creator->lib.mod_imp, "sf") != 0) {
        printf("Error: sf graphics requires a window created by the sf module\n");
        printf("Got a window created by '%s'\n", win->creator->lib.mod_imp);
        return;
    }
    __sfWindow = win;
    auto* window = (sf::RenderWindow*)win->internal;
    window->setActive(true);

}

#include "opengl.h"

