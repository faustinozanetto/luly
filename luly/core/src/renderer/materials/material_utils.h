﻿#pragma once

#include "material.h"

namespace luly::renderer
{
    class material_utils
    {
    public:
        static const char* get_material_texture_type_to_string(material_texture_type texture_type);
        static const char* get_material_texture_bind_location(material_texture_type texture_type);
        static const char* get_material_texture_channel_location(material_texture_type texture_type);
    };
}
