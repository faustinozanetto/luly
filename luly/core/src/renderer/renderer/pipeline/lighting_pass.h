#pragma once

#include "renderer/renderer/pass/render_pass.h"

namespace luly::renderer
{
    class lighting_pass : public render_pass
    {
    public:
        lighting_pass();
        ~lighting_pass() override;

        /* Overrides */
        void initialize() override;
    };
}
