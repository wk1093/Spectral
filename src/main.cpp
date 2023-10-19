#include "modules/scriptloading/module.h"
#include "modules/window/module.h"
#include "modules/engineui/module.h"

int main() {
    // string array
//    ScriptLoaderModule module("modules/cppscript.so"); // we load the cppscript module
//    printf("input extension: %s\n", module.inputExtension); // prints cpp
//    printf("output extension: %s\n", module.outputExtension); // prints so on linux, dll on windows
//
//    const char* files[] = {"scripts/testScript.cpp", "scripts/testScript2.cpp"};
//    module.compileScripts(files, 2, "testScripts.so", "../src/include");
//
//    Script script = module.loadScript("./testScripts.so", "testScript2");
//    script.init();
//    script.update(2);

    WindowModule winm("modules/sfwin.so");

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
