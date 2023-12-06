#pragma once

#include "panels/scene/actor/components/actor_component_panel.h"

#include <glm/glm.hpp>

namespace luly::ui
{
    class actor_directional_light_component_panel : public actor_component_panel
    {
    public:
        actor_directional_light_component_panel();

        /* Overrides */
        std::pair<bool, size_t> get_actor_component_details() override;
        void on_render_component_details() override;

    private:
        int m_selected_dimensions_index;
        std::vector<std::string> m_shadow_map_dimension_names = {"1024x1024", "2048x2048", "4096x4096", "8192x8192"};
        std::vector<glm::ivec2> m_shadow_map_dimensions = {{1024, 1024}, {2048, 2048}, {4096, 4096}, {8192, 8192}};
    };
}
