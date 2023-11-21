#pragma once

#include "renderer_api.h"
#include "perspective/perspective_camera.h"
#include "perspective/perspective_camera_controller.h"

#include <memory>

namespace luly::renderer
{
    class LULY_RENDERER_API camera_manager
    {
    public:
        camera_manager();
        ~camera_manager();

        /* Getters */
        const std::shared_ptr<perspective_camera>& get_perspective_camera() const { return m_perspective_camera; }

        const std::shared_ptr<perspective_camera_controller>& get_perspective_camera_controller() const
        {
            return m_perspective_camera_controller;
        }

    private:
        std::shared_ptr<perspective_camera> m_perspective_camera;
        std::shared_ptr<perspective_camera_controller> m_perspective_camera_controller;
    };
}
