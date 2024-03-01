#include "lypch.h"
#include "physics_error_callback.h"

namespace luly::physics
{
    physics_error_callback::physics_error_callback()
    {
    }

    physics_error_callback::~physics_error_callback()
    {
    }

    void physics_error_callback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file,
                                             int line)
    {
        LY_ERROR("PhysX error reported:");
        LY_ERROR("  - Code: {}", static_cast<int>(code));
        LY_ERROR("  - Message: {}", message);
        LY_ERROR("  - File: {}", file);
        LY_ERROR("  - Line: {}", line);
    }
}
