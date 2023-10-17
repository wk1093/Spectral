#include "script.h"


int main() {
    // string array
    const char* files[] = {"scripts/testScript.cpp", "scripts/testScript2.cpp"};
    compileScripts(files, 2, "testScript.so");

    Script script = loadScript("testScript.so");
    script.init();
    script.update(2);

    return 0;

}
