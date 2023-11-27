#include "assets_panel.h"

#include <time/app_time.h>

#include "assets/assets_manager.h"
#include "assets/asset_utils.h"

namespace luly::ui
{
    bool assets_panel::s_show = true;

    assets_panel::assets_panel() : ui_panel("assets_panel")
    {
    }

    assets_panel::~assets_panel()
    {
    }

    void assets_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Assets", &s_show))
        {
            assets::assets_manager& assets_manager = assets::assets_manager::get();

            for (const auto& [type, assets] : assets_manager.get_assets())
            {
                // Display a header for each asset type
                const std::string label = std::format("Asset Type: {}",
                                                      assets::asset_utils::get_asset_type_to_string(type));
                if (ImGui::TreeNodeEx(label.c_str()))
                {
                    // Iterate through assets of the current type
                    for (const auto& asset : assets)
                    {
                        if (ImGui::TreeNode(asset->get_metadata().name.c_str()))
                        {
                            ImGui::BulletText("Name: %s", asset->get_metadata().name.c_str());
                            ImGui::BulletText("UUID: %s", std::to_string(asset->get_metadata().uuid).c_str());

                            ImGui::TreePop();
                        }
                    }

                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }

    bool assets_panel::get_show_panel()
    {
        return s_show;
    }

    void assets_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
