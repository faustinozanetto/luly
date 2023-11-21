#include "camera_manager.h"

namespace luly::renderer
{
    camera_manager::camera_manager()
    {
        m_perspective_camera = std::make_shared<perspective_camera>();
        m_perspective_camera_controller = std::make_shared<perspective_camera_controller>(m_perspective_camera);
    }

    camera_manager::~camera_manager()
    {
    }
}
