#pragma once

#include "renderer_api.h"

namespace luly::renderer
{
    class LULY_RENDERER_API bindable_object
    {
    public:
        virtual ~bindable_object() = default;

        virtual void bind() = 0;
        virtual void un_bind() = 0;
    };
}
