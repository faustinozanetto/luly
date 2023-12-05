#pragma once

#include "texture.h"

namespace luly::renderer
{
    class texture_3d : public texture
    {
    public:
        texture_3d(const texture_specification& texture_specification);

        /* Overrides */
        void initialize() override;
    };
}
