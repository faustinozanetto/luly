#include "lypch.h"
#include "geometry_pass.h"

#include "renderer/renderer/renderer.h"

namespace luly::renderer
{
    geometry_pass::geometry_pass() : render_pass("geometry_pass")
    {
        geometry_pass::initialize();
    }

    geometry_pass::~geometry_pass()
    {
    }

    void geometry_pass::initialize()
    {
        auto viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            // Position
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
            // Albedo
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
            // Normal
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
            // Roughness-Metallic-AO
            {
                texture_internal_format::rgba16f,
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
