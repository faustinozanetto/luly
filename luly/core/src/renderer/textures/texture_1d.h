#pragma once

#include "texture.h"

namespace luly::renderer
{
    class texture_1d : public texture
    {
    public:
        texture_1d(const texture_specification& texture_specification);

        /* Overrides */
        void initialize() override;
    };
}
