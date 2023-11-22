#pragma once

#ifdef _WIN32
        #ifdef LULY_UI_EXPORTS
                #define LULY_UI_API
        #else
                #define LULY_UI_API
        #endif
#else
    #define LULY_UI_API
#endif