﻿#include "final_pass.h"

#include "renderer/renderer.h"

namespace luly::renderer
{
    final_pass::final_pass() : render_pass("final_pass")
    {
        final_pass::initialize();
    }

    final_pass::~final_pass()
    {
    }

    void final_pass::initialize()
    {
        auto viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            {
                texture_internal_format::rgba8,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
        };

        frame_buffer_attachment depth_attachment = {
            texture_internal_format::depth_component32f,
            texture_filtering::linear,
            texture_wrapping::clamp_to_edge, viewport_size
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments, depth_attachment);
    }
}