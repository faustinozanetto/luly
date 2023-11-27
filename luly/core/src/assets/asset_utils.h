#pragma once

#include "asset.h"

namespace luly::assets
{
    class asset_utils
    {
    public:
        static const char* get_asset_type_to_string(asset_type type);
    };
}
