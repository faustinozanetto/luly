#pragma once

#ifdef LULY_SHARED_EXPORTS
#define LULY_SHARED_API __declspec(dllexport)
#else
        #define LULY_SHARED_API __declspec(dllimport)
#endif
