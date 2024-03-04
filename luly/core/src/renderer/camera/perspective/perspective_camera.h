#pragma once

#include "renderer/camera/camera.h"

namespace luly::renderer
{
    class perspective_camera : public camera
    {
    public:
        perspective_camera(float fov = 45.0f);
        ~perspective_camera() override;

        /* Overrides */
        void update_view_matrix() override;
        void update_projection_matrix(const glm::ivec2& dimensions) override;

        /* Getters*/
        float get_fov() const { return m_fov; }
        std::vector<glm::vec4> get_frustum_corners_world_space(const glm::mat4& projection_matrix,
                                                               const glm::mat4& view_matrix);

        /* Setters */
        void set_fov(float fov);

    private:
        float m_fov;
    };
}
