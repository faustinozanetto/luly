#pragma once

#ifdef LULY_CORE_EXPORTS
#define LULY_CORE_API __declspec(dllexport)
#else
#define LULY_CORE_API __declspec(dllimport)
#endif
