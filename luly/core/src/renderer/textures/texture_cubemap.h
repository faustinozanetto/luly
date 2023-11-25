#pragma once
#include "texture.h"

namespace luly::renderer
{
    class texture_cubemap : public texture
    {
    public:
        texture_cubemap(const texture_specification& texture_specification);

        /* Overrides */
        void initialize() override;
    };
}
