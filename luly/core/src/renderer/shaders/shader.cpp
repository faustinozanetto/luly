#include "lypch.h"
#include "shader.h"

#include "shader_utils.h"

#include <glm/gtc/type_ptr.hpp>

#include "shader_factory.h"

namespace luly::renderer
{
    shader::shader(const std::unordered_map<shader_type, std::string>& shader_contents, const std::string& file_path)
    {
        m_file_path = file_path;
        m_contents = shader_contents;
        initialize();
    }

    shader::~shader()
    {
        glDeleteProgram(m_handle);
    }

    int shader::get_uniform_location(const std::string& uniform_name)
    {
        if (m_uniforms.contains(uniform_name))
            return m_uniforms[uniform_name];

        int location = glGetUniformLocation(m_handle, uniform_name.c_str());
        m_uniforms.insert(std::make_pair(uniform_name, location));
        return location;
    }

    std::string shader::get_name() const
    {
        const std::filesystem::path parsed_path(m_file_path);
        return parsed_path.stem().string();
    }

    void shader::bind()
    {
        glUseProgram(m_handle);
    }

    void shader::un_bind()
    {
        glUseProgram(0);
    }

    void shader::recompile()
    {
        LY_TRACE("Started recompiling shader: '{}'...", get_name());

        // Clear actual content and re extract using factory.
        m_contents.clear();
        m_contents = shader_factory::extract_shader_contents(m_file_path);
        initialize();

        LY_TRACE("Shader recompiled successfully!");
    }

    void shader::set_int(const std::string& uniform_name, int value)
    {
        glUniform1i(get_uniform_location(uniform_name), value);
    }

    void shader::set_vec_int2(const std::string& uniform_name, const glm::ivec2& value)
    {
        glUniform2i(get_uniform_location(uniform_name), value.x, value.y);
    }

    void shader::set_vec_int3(const std::string& uniform_name, const glm::ivec3& value)
    {
        glUniform3i(get_uniform_location(uniform_name), value.x, value.y, value.z);
    }

    void shader::set_vec_int4(const std::string& uniform_name, const glm::ivec4& value)
    {
        glUniform4i(get_uniform_location(uniform_name), value.x, value.y, value.z, value.w);
    }

    void shader::set_float(const std::string& uniform_name, float value)
    {
        glUniform1f(get_uniform_location(uniform_name), value);
    }

    void shader::set_vec_float2(const std::string& uniform_name, const glm::vec2& value)
    {
        glUniform2f(get_uniform_location(uniform_name), value.x, value.y);
    }

    void shader::set_vec_float3(const std::string& uniform_name, const glm::vec3& value)
    {
        glUniform3f(get_uniform_location(uniform_name), value.x, value.y, value.z);
    }

    void shader::set_vec_float4(const std::string& uniform_name, const glm::vec4& value)
    {
        glUniform4f(get_uniform_location(uniform_name), value.x, value.y, value.z, value.w);
    }

    void shader::set_mat3(const std::string& uniform_name, const glm::mat3& value)
    {
        glUniformMatrix3fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void shader::set_mat4(const std::string& uniform_name, const glm::mat4& value)
    {
        glUniformMatrix4fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void shader::initialize()
    {
        LY_TRACE("Started initializing shader...");

        create_program();
        const std::vector<uint32_t> shader_ids = create_shaders();
        link_program();
        check_for_link_errors(shader_ids);

        LY_TRACE("Shader initialized successfully!");
    }

    void shader::create_program()
    {
        m_handle = glCreateProgram();
    }

    std::vector<uint32_t> shader::create_shaders() const
    {
        std::vector<uint32_t> shader_ids;
        int shader_index = 0;

        for (auto& shader : m_contents)
        {
            uint32_t shader_id = glCreateShader(shader_utils::get_shader_type_to_opengl(shader.first));
            const char* source_str = shader.second.c_str();
            glShaderSource(shader_id, 1, &source_str, nullptr);
            glCompileShader(shader_id);

            int success = 0;
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                int max_length = 0;
                glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

                std::vector<char> infoLog(max_length);
                glGetShaderInfoLog(shader_id, max_length, &max_length, infoLog.data());

                glDeleteShader(shader_id);

                auto error_message = std::string(
                    infoLog.begin(), infoLog.end());
                LY_ERROR("An error occurred while compiling shader '{}'", error_message);
                LY_ASSERT_MSG(false, "Shader compilation failed!")
            }

            LY_TRACE("  - Shader '{}' compiled successfully.", shader_utils::get_shader_type_to_string(shader.first));

            glAttachShader(m_handle, shader_id);
            shader_ids.push_back(shader_id);
            shader_index++;
        }

        return shader_ids;
    }

    void shader::link_program() const
    {
        glLinkProgram(m_handle);
    }

    void shader::check_for_link_errors(const std::vector<uint32_t>& shader_ids) const
    {
        int success = 0;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            int max_lenght = 0;
            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &max_lenght);

            std::vector<char> infoLog(max_lenght);
            glGetProgramInfoLog(m_handle, max_lenght, &max_lenght, infoLog.data());

            glDeleteProgram(m_handle);

            for (const uint32_t id : shader_ids)
            {
                glDeleteShader(id);
            }

            auto error_message = std::string(
                infoLog.begin(), infoLog.end());
            LY_ERROR("An error ocurred while linking shader '{}'", error_message);
            LY_ASSERT_MSG(false, "Shader linking failed!")
        }

        for (const auto id : shader_ids)
        {
            glDetachShader(m_handle, id);
            glDeleteShader(id);
        }
    }
}
