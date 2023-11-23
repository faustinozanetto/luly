#pragma once

#include "mesh.h"

namespace luly::renderer
{
    class mesh_factory
    {
    public:
        static std::shared_ptr<mesh> create_screen_quad_mesh();
        static std::shared_ptr<mesh> create_cube_mesh();
    };
}
