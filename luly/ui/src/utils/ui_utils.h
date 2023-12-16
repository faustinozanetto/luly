#pragma once

#include <functional>

#include "renderer/textures/texture.h"
#include "input/mouse_codes.h"

#include <imgui.h>
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace luly::ui
{
    class ui_utils
    {
    public:
        static void draw_tooltip(const char* text);
        static void draw_property(const std::string& name);
        static void draw_property(const std::string& name, const std::string& content);
        static bool draw_property(const std::string& name, bool& value);
        static bool draw_property(const std::string& name, int& value, int min, int max, int reset_value = 1.0f);
        static bool draw_property(const std::string& name, float& value, float min, float max, float step = 0.1f,
                                  float reset_value = 1.0f);
        static bool draw_property(const std::string& name, glm::vec2& value, float min, float max, float step = 0.1f,
                                  float reset_value = 1.0f);
        static bool draw_property(const std::string& name, glm::vec3& value, float min, float max, float step = 0.1f,
                                  float reset_value = 1.0f);
        static bool draw_property(const std::string& name, glm::vec3& value, bool color = true);
        static bool draw_property(const std::string& name, glm::vec4& value, bool color = true);
        static bool draw_property(const std::string& name, const std::shared_ptr<renderer::texture>& texture,
                                  const ImVec2& image_size = ImVec2(64, 64), bool flip_image = true);
        static bool draw_combo_box(const std::string& name, const std::vector<std::string>& options,
                                   const std::function<void(const std::string& selected_option, int option_index)>&
                                   selected_callback);
        static bool draw_drop_down(const std::string& name, const char** options, int32_t option_count, int32_t* selected);
        static bool draw_property(uint32_t texture_handle,
                                  const ImVec2& image_size = ImVec2(64, 64), bool flip_image = true);

        static int get_mouse_button_code_to_imgui(input::mouse_button mouse_button);
    };
}
