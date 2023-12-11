#pragma once

#include "directional_light_shadows_manager.h"
#include "point_light_shadows_manager.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/lights/point_light.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/textures/texture_2d.h"

#include <memory>

namespace luly::renderer
{
    struct shadows_data
    {
        bool soft_shadows;
        int pcf_horizontal_samples;
        int pcf_vertical_samples;
    };

    class shadows_pass : public render_pass
    {
    public:
        shadows_pass();
        ~shadows_pass() override;

        /* Getters */
        std::shared_ptr<directional_light_shadows_manager>& get_directional_light_shadows_manager()
        {
            return m_directional_light_shadows_manager;
        }

        std::shared_ptr<point_light_shadows_manager>& get_point_light_shadows_manager()
        {
            return m_point_light_shadows_manager;
        }

        shadows_data& get_shadows_data() { return m_shadows_data; }

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

        /* Methods */
        void bind_uniforms(const std::shared_ptr<shader>& shader) const;

    private:
        void initialize_shadows_data();

        std::shared_ptr<directional_light_shadows_manager> m_directional_light_shadows_manager;
        std::shared_ptr<point_light_shadows_manager> m_point_light_shadows_manager;

        shadows_data m_shadows_data;
    };
}
