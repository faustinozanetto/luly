#include "lypch.h"
#include "assets_manager.h"

#include <logging/log.h>
#include <utils/assert.h>

namespace luly::assets
{
    assets_manager* assets_manager::s_instance = nullptr;

    assets_manager::assets_manager()
    {
        LY_INFO("Started creating assets manager...");
        LY_INFO("Assets manager created successfully!");
    }

    assets_manager& assets_manager::get()
    {
        LY_ASSERT_MSG(s_instance, "Assets manager instance is null!")
        return *s_instance;
    }

    void assets_manager::initialize()
    {
        s_instance = new assets_manager();
    }

    void assets_manager::register_asset(const std::shared_ptr<asset>& asset)
    {
        LY_ASSERT_MSG(!asset_already_registered(asset->get_metadata().name), "Asset is already registered!")

        m_assets_map[asset->get_metadata().type].push_back(asset);
    }

    bool assets_manager::asset_already_registered(const std::string& asset_name)
    {
        for (const std::pair<const asset_type, std::vector<std::shared_ptr<asset>>>& asset_pair : m_assets_map)
        {
            for (const std::shared_ptr<asset>& asset : asset_pair.second)
            {
                if (asset->get_metadata().name == asset_name) return true;
            }
        }

        return false;
    }
}
