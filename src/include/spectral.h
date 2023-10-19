#pragma once

#ifdef _WIN32
    #ifndef _STATIC_SCRIPTS
        #define EXPORT __declspec(dllexport)
    #else // _STATIC_SCRIPTS
        #define EXPORT
    #endif // _STATIC_SCRIPTS
#else // _WIN32
    #define EXPORT
#endif // _WIN32

// this will contain/include the script interface, ECS and stuff like that,
// anything the user needs to access will be included here