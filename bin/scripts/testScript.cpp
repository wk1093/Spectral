#include "spectral.h"

#include <stdio.h>

namespace testScript {
        EXPORT void init() { // maybe a context object like EXPORT void init(InitContext& c) if needed
            printf("Hello, World!\n");
        }

        EXPORT void update(float dt) {
            printf("Hello, Update!\n");
        }
}