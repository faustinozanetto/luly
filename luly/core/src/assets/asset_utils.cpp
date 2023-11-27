#include "lypch.h"
#include "asset_utils.h"

namespace luly::assets
{
    const char* asset_utils::get_asset_type_to_string(const asset_type type)
    {
        switch (type)
        {
        case asset_type::shader:
            return "shader";
        case asset_type::texture:
            return "texture";
        case asset_type::model:
            return "model";
        case asset_type::material:
            return "material";
        }
        LY_ASSERT_MSG(false, "Invalid asset type!")
        return nullptr;
    }
}
