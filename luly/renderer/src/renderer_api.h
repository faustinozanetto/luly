#pragma once

#ifdef _WIN32
    #ifdef LULY_RENDERER_EXPORTS
        #define LULY_RENDERER_API __declspec(dllexport)
    #else
        #define LULY_RENDERER_API __declspec(dllimport)
    #endif
#else
    #define LULY_RENDERER_API
#endif