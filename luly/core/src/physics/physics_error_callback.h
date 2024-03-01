#pragma once

#include <foundation/PxErrorCallback.h>

namespace luly::physics
{
    class physics_error_callback : public physx::PxErrorCallback
    {
    public:
        physics_error_callback();
        ~physics_error_callback() override;

        virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file,
                                 int line) override;
    };
}
