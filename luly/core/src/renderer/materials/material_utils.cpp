#include "lypch.h"
#include "material_utils.h"

#include <utils/assert.h>
#include <logging/log.h>

namespace luly::renderer
{
    const char* material_utils::get_material_texture_type_to_string(material_texture_type texture_type)
    {
        switch (texture_type)
        {
        case material_texture_type::albedo:
            return "albedo";
        case material_texture_type::normal:
            return "normal";
        case material_texture_type::roughness:
            return "roughness";
        case material_texture_type::metallic:
            return "metallic";
        case material_texture_type::ambient_occlusion:
            return "ambient_occlusion";
        case material_texture_type::opacity:
            return "opacity";
        }
        LY_ASSERT_MSG(false, "Invalid material texture type!");
        return nullptr;
    }
}
