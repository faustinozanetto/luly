#pragma once

#include "renderer_api.h"
#include "camera/camera.h"

namespace luly::renderer
{
    class LULY_RENDERER_API perspective_camera : public camera
    {
    public:
        perspective_camera(float fov = 45.0f);
        ~perspective_camera() override;

        /* Overrides */
        void update_view_matrix() override;
        void update_projection_matrix(const glm::ivec2& dimensions) override;

        /* Getters*/
        float get_fov() const { return m_fov; }

        /* Setters */
        void set_fov(float fov);

    private:
        float m_fov;
    };
}
