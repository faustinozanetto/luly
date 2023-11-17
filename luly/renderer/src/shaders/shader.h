#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common/bindable_object.h"

namespace luly::renderer
{
    enum class shader_type
    {
        fragment,
        vertex,
        geometry,
    };

    class shader : public bindable_object
    {
    public:
        shader(const std::unordered_map<shader_type, std::string>& shader_contents);
        ~shader() override;

        /* Getters */
        uint32_t get_handle() const { return m_handle; }
        uint32_t get_uniform_location(const std::string& uniform_name);

        void bind() override;
        void un_bind() override;

    private:
        void initialize();
        void create_program();
        std::vector<uint32_t> create_shaders();
        void link_program() const;
        void check_for_link_errors(const std::vector<uint32_t>& shader_ids) const;

        uint32_t m_handle;
        std::unordered_map<std::string, uint32_t> m_uniforms;
        std::unordered_map<shader_type, std::string> m_contents;
    };
}
