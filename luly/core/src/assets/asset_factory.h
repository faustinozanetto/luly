#pragma once

#include "asset.h"
#include "assets_manager.h"
#include "asset_utils.h"

namespace luly::assets
{
    class asset_factory
    {
    public:
        template <typename T>
        static std::shared_ptr<asset> create_asset(const std::string& name, const asset_type type,
                                                   const std::shared_ptr<T>& data)
        {
            asset_metadata metadata = {type, name};
            // Create asset and set data.
            const std::shared_ptr<asset>& created_asset = std::make_shared<asset>(metadata);
            created_asset->set_data(data);

            // Register asset in manager.
            assets_manager::get().register_asset(created_asset);

            LY_INFO("Created asset with name: '{0}' and type: '{1}'!", name,
                    asset_utils::get_asset_type_to_string(type));

            return created_asset;
        }
    };
}
