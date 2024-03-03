#pragma once

#include <memory>

#include "panels/ui_panel.h"
#include "renderer/shaders/shader.h"

namespace luly::ui
{
    class shaders_panel : public ui_panel
    {
    public:
        shaders_panel();
        ~shaders_panel() override;

        /* Overrides */
        void on_render_panel() override;
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

    private:
        void read_shader_contents_from_file();
        void save_shader_changes_to_file() const;
        static int shader_input_resize_callback(ImGuiInputTextCallbackData* data);

        std::shared_ptr<renderer::shader> m_selected_shader;
        std::vector<char> m_shader_buffer;
        static bool s_show;
    };
}
