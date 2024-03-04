#include "shaders_panel.h"

#include <fstream>

#include "engine_ui.h"
#include "assets/assets_manager.h"
#include "renderer/shaders/shader.h"
#include "renderer/shaders/shader_factory.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    bool shaders_panel::s_show = true;

    shaders_panel::shaders_panel() : ui_panel("shaders_panel")
    {
    }

    shaders_panel::~shaders_panel()
    {
    }

    void shaders_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Shaders", &s_show))
        {
            const std::vector<std::shared_ptr<renderer::shader>>& shader_assets = assets::assets_manager::get().
                get_assets_by_type<renderer::shader, assets::asset_type::shader>();

            for (const std::shared_ptr<renderer::shader>& shader : shader_assets)
            {
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_FramePadding;
                flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
                if (ImGui::TreeNodeEx(shader->get_name().c_str(), flags))
                {
                    if (ImGui::IsItemClicked())
                    {
                        m_selected_shader = shader;
                        read_shader_contents_from_file();
                    }
                    ui_utils::draw_property("File Path", shader->get_file_path());
                    ui_utils::draw_property("Handle", std::format("{}", shader->get_handle()));
                    if (ImGui::Button("Recompile"))
                    {
                        shader->recompile();
                    }
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();

        if (m_selected_shader)
        {
            if (ImGui::Begin("Shader Editor"))
            {
                ui_utils::draw_property("Currently Editing", m_selected_shader->get_name());
                if (ImGui::InputTextMultiline("##Shader Editor", m_shader_buffer.data(), m_shader_buffer.size(),
                                              ImVec2(-1.0f, -1.0f),
                                              ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackResize,
                                              shader_input_resize_callback, &m_shader_buffer))
                {
                }
                if (ImGui::Button("Save Changes"))
                {
                    save_shader_changes_to_file();
                }
            }
            ImGui::End();
        }
    }

    int shaders_panel::shader_input_resize_callback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            // Resize string callback
            std::vector<char>* buffer = static_cast<std::vector<char>*>(data->UserData);
            if (buffer)
            {
                buffer->resize(data->BufTextLen + 1); // +1 for null terminator
                data->Buf = buffer->data();
            }
        }
        return 0;
    }

    void shaders_panel::read_shader_contents_from_file()
    {
        std::string shader_contents = renderer::shader_factory::read_shader_from_file(
            m_selected_shader->get_file_path());
        m_shader_buffer.resize(shader_contents.size() + 1);
        std::ranges::copy(shader_contents, m_shader_buffer.begin());
        m_shader_buffer[shader_contents.size()] = '\0';
    }

    void shaders_panel::save_shader_changes_to_file() const
    {
        LY_TRACE("Started saving shader changes to file...");
        std::ofstream shader_file(m_selected_shader->get_file_path(), std::ios::out);
        LY_ASSERT_MSG(shader_file.is_open(), "Failed to open shader file!")

        shader_file << m_shader_buffer.data();

        shader_file.close();
        LY_TRACE("Shader changes saved to file successfully!");
    }


    bool shaders_panel::get_show_panel()
    {
        return s_show;
    }

    void shaders_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
