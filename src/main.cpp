#include "modules/scrld/module.h"
#include "modules/win/module.h"

int main() {
//    ScriptLoaderModule module("cpp"); // we load the cppscript module
//    printf("input extension: %s\n", module.inputExtension); // prints cpp
//    printf("output extension: %s\n", module.outputExtension); // prints so on linux, dll on windows
//
//    const char* files[] = {"scripts/testScript.cpp", "scripts/testScript2.cpp"};
//    module.compileScripts(files, 2, "testScripts.so", "../src/include");
//
//    Script script = module.loadScript("./testScripts.so", "testScript2");
//    script.init();
//    script.update(2);

    WindowModule winm("sf");
    if (!winm.lib.valid()) {
        printf("Error loading window module\n");
        return 1;
    }

    sWindow win = winm.loadWindow("test", 800, 600);

    while (!winm.shouldClose(win)) {
        winm.updateWindow(win);

        if (winm.isKeyPressed(win, Key::A)) {
            printf("A pressed\n");
        }

        winm.swapBuffers(win);
    }

    winm.destroyWindow(win);

    return 0;

}
