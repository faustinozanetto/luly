#pragma once

#ifdef LULY_UI_EXPORTS
#define LULY_UI_API __declspec(dllexport)
#else
        #define LULY_UI_API __declspec(dllimport)
#endif
