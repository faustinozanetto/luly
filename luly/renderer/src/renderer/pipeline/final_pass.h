#pragma once
#include "renderer/pass/render_pass.h"

namespace luly::renderer
{
    class final_pass : public render_pass
    {
    public:
        final_pass();
        ~final_pass() override;

        /* Overrides */
        void initialize() override;
    };
}
