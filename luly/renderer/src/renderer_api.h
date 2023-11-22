#pragma once

#ifdef _WIN32
    #ifdef LULY_RENDERER_EXPORTS
        #define LULY_RENDERER_API
    #else
        #define LULY_RENDERER_API
    #endif
#else
    #define LULY_RENDERER_API
#endif