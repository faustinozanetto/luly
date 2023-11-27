#pragma once

#include "assets/asset.h"

#include <logging/log.h>
#include <utils/assert.h>

namespace luly::assets
{
    class assets_manager
    {
    protected:
        assets_manager();

    public:
        /* Getters */
        static assets_manager& get();
        const std::unordered_map<asset_type, std::vector<std::shared_ptr<asset>>>& get_assets() { return m_assets_map; }

        template <class T, asset_type P>
        std::shared_ptr<T> get_asset(const std::string& asset_name)
        {
            std::shared_ptr<T> found_asset;
            for (const std::pair<asset_type, std::vector<std::shared_ptr<asset>>> asset_category : m_assets_map)
            {
                if (asset_category.first == P)
                {
                    for (const std::shared_ptr<asset>& asset : asset_category.second)
                    {
                        found_asset = std::dynamic_pointer_cast<T>(asset);
                        if (found_asset && asset_name.compare(found_asset->get_metadata().name) == 0)
                        {
                            return found_asset;
                        }
                    }
                }
            }
            LY_ERROR("Failed to find asset with name '{0}'!", asset_name);
            LY_ASSERT(false);

            return nullptr;
        }

        /* Methods */
        static void initialize();
        void register_asset(const std::shared_ptr<asset>& asset);
        bool asset_already_registered(const std::string& asset_name);

    private:
        std::unordered_map<asset_type, std::vector<std::shared_ptr<asset>>> m_assets_map;

        /* Singleton Instance */
        static assets_manager* s_instance;
    };
}
