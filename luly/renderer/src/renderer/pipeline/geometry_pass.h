#pragma once
#include "renderer/pass/render_pass.h"

namespace luly::renderer
{
    class geometry_pass : public render_pass
    {
    public:
        geometry_pass();
        ~geometry_pass() override;

        /* Overrides */
        void initialize() override;
    };
}
