#include "spectral.h"

#include <stdio.h>

namespace testScript2 {
        EXPORT void init() { // maybe a context object like EXPORT void init(InitContext& c) if needed
            printf("Hello, World 2!\n");
        }

        EXPORT void update(float dt) {
            printf("Hello, Update 2!\n");
        }
}