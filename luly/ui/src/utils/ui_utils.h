#pragma once

#include <string>
#include <glm/glm.hpp>

namespace luly::ui
{
    class ui_utils
    {
    public:
        static bool draw_property(const std::string& name, glm::vec2& value, float min, float max, float step = 0.1f,
                                  float reset_value = 1.0f);
        static bool draw_property(const std::string& name, glm::vec3& value, float min, float max, float step = 0.1f,
                                  float reset_value = 1.0f);
        static bool draw_property(const std::string& name, glm::vec3& value, bool color = true);
        static bool draw_property(const std::string& name, glm::vec4& value, bool color = true);
    };
}
