#include "modules/scriptloading/module.h"

int main() {
    // string array
    ScriptLoaderModule module("modules/cppscript.so"); // we load the cppscript module
    printf("input extension: %s\n", module.inputExtension); // prints cpp
    printf("output extension: %s\n", module.outputExtension); // prints so on linux, dll on windows

    const char* files[] = {"scripts/testScript.cpp", "scripts/testScript2.cpp"};
    module.compileScripts(files, 2, "testScript.so");

    Script script = module.loadScript("./testScript.so");
    script.init();
    script.update(2);

    return 0;

}
