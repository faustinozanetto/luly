#pragma once
#include "camera.h"

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

    private:
        float m_fov;
    };
}
