﻿#pragma once

#include "common/bindable_object.h"
#include "renderer_api.h"

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace luly::renderer
{
    enum class LULY_RENDERER_API shader_type
    {
        fragment,
        vertex,
        geometry,
    };

    class LULY_RENDERER_API shader : public bindable_object
    {
    public:
        shader(const std::unordered_map<shader_type, std::string>& shader_contents);
        ~shader() override;

        /* Getters */
        uint32_t get_handle() const { return m_handle; }
        uint32_t get_uniform_location(const std::string& uniform_name);

        /* Overrides */
        void bind() override;
        void un_bind() override;

        /* Methods */
        void set_int(const std::string& uniform_name, int value);
        void set_vec_int2(const std::string& uniform_name, const glm::ivec2& value);
        void set_vec_int3(const std::string& uniform_name, const glm::ivec3& value);
        void set_vec_int4(const std::string& uniform_name, const glm::ivec4& value);
        void set_float(const std::string& uniform_name, float value);
        void set_vec_float2(const std::string& uniform_name, const glm::vec2& value);
        void set_vec_float3(const std::string& uniform_name, const glm::vec3& value);
        void set_vec_float4(const std::string& uniform_name, const glm::vec4& value);
        void set_mat3(const std::string& uniform_name, const glm::mat3& value);
        void set_mat4(const std::string& uniform_name, const glm::mat4& value);

    private:
        void initialize();
        void create_program();
        std::vector<uint32_t> create_shaders() const;
        void link_program() const;
        void check_for_link_errors(const std::vector<uint32_t>& shader_ids) const;

        uint32_t m_handle;
        std::unordered_map<std::string, uint32_t> m_uniforms;
        std::unordered_map<shader_type, std::string> m_contents;
    };
}
