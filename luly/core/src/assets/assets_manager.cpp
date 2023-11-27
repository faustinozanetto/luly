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
        m_assets_map[asset->get_metadata().type].push_back(asset);
    }
}
