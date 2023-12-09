#include "lypch.h"
#include "directional_light_shadows_manager.h"

#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    directional_light_shadows_manager::directional_light_shadows_manager()
    {
        initialize();
    }

    directional_light_shadows_manager::~directional_light_shadows_manager()
    {
    }

    void directional_light_shadows_manager::set_debug_cascade_color(int cascade_index, const glm::vec3& color)
    {
        LY_ASSERT_MSG(cascade_index >= 0 && cascade_index < CASCADES_COUNT, "Cascade index out of range!")
        m_cascade_debug_colors[cascade_index] = glm::vec4(color, 1.0);
    }

    void directional_light_shadows_manager::initialize()
    {
        m_directional_light_shadows_shader = shader_factory::create_shader_from_file(
            "assets/shaders/shadows/directional_light_shadows.lsh");

        initialize_shadows_data();
    }

    void directional_light_shadows_manager::initialize_shadows_data()
    {
        m_directional_light_shadows_data.show_cascades = 0;
        m_directional_light_shadows_data.shadow_bias = 0.0f;
        m_directional_light_shadows_data.cascade_debug_colors[0] = glm::vec4(0.92f, 0.92f, 0.1f, 1);
        m_directional_light_shadows_data.cascade_debug_colors[1] = glm::vec4(0.1f, 0.83f, 0.82f, 1);
        m_directional_light_shadows_data.cascade_debug_colors[2] = glm::vec4(0.87f, 0.12f, 0.62f, 1);
        m_directional_light_shadows_data.cascade_plane_distances[0] = 0.0f;
        m_directional_light_shadows_data.cascade_plane_distances[1] = 0.0f;
        m_directional_light_shadows_data.cascade_plane_distances[2] = 0.0f;
    }
}
