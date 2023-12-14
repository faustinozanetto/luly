#pragma once

#ifdef LY_PROFILING
#define TRACY_CALLSTACK 1

#include <tracy/Tracy.hpp>
#define LY_PROFILE_SCOPE(name) ZoneScopedN(name)
#define LY_PROFILE_FUNCTION ZoneScoped
#define LY_PROFILE_FRAMEMARKER() FrameMark
#define LY_PROFILE_LOCK(type, var, name) TracyLockableN(type, var, name)
#include <tracy/TracyOpenGL.hpp>
#define LY_PROFILE_COLLECT TracyGpuCollect
#define LY_PROFILE_CONTEXT TracyGpuContext
#else
#define LY_PROFILE_SCOPE(name)
#define LY_PROFILE_FUNCTION
#define LY_PROFILE_FRAMEMARKER()
#define LY_PROFILE_LOCK(type, var, name) type var
#define LY_PROFILE_COLLECT
#define LY_PROFILE_CONTEXT
#endif
