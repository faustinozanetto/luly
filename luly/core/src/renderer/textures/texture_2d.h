#pragma once
#include "texture.h"

namespace luly::renderer
{
    class texture_2d : public texture
    {
    public:
        texture_2d(const texture_specification& texture_specification);

        /* Overrides */
        void initialize() override;
    };
}
