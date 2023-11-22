﻿#include "lypch.h"
#include "shader_factory.h"

#include "shader_utils.h"

#include <logging/log.h>
#include <utils/assert.h>
#include <fstream>

namespace luly::renderer
{
    std::shared_ptr<shader> shader_factory::create_shader_from_file(const std::string& file_path)
    {
        LY_TRACE("Started loading shader from file '{0}'", file_path);

        const std::string& shader_source = read_shader_from_file(file_path);
        const auto& contents = parse_shader_source(shader_source);

        auto created_shader = std::make_shared<shader>(contents);

        LY_TRACE("Shader loaded from file successfully!");
        return created_shader;
    }

    std::string shader_factory::read_shader_from_file(const std::string& file_path)
    {
        LY_TRACE("Started reading shader from file '{0}'", file_path);

        std::string shader_contents;
        std::ifstream in(file_path, std::ios::in | std::ios::binary);

        LY_ASSERT_MSG(in, "An error occurred while opening shader file '{0}'", file_path);

        in.seekg(0, std::ios::end);
        const size_t size = in.tellg();
        LY_ASSERT_MSG(size != -1, "An error occurred while reading shader file '{0}'", file_path);
        shader_contents.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(shader_contents.data(), size);

        in.close();

        LY_TRACE("Shader contents read successfully.");
        return shader_contents;
    }

    std::unordered_map<shader_type, std::string> shader_factory::parse_shader_source(const std::string& shader_source)
    {
        LY_TRACE("Started parsing shader sources...");
        std::unordered_map<shader_type, std::string> shader_contents;

        const size_t shader_type_token_len = strlen(SHADER_TYPE_TOKEN);
        size_t pos = shader_source.find(SHADER_TYPE_TOKEN, 0);

        auto current_type = shader_type::fragment;

        while (pos != std::string::npos)
        {
            const size_t eol = shader_source.find_first_of("\r\n", pos);
            const size_t begin = pos + shader_type_token_len + 1;
            std::string type = shader_source.substr(begin, eol - begin);

            const size_t next_line_pos = shader_source.find_first_not_of("\r\n", eol);
            pos = shader_source.find(SHADER_TYPE_TOKEN, next_line_pos);

            if (type == shader_utils::get_shader_type_to_string(shader_type::vertex))
                current_type = shader_type::vertex;
            else if (type == shader_utils::get_shader_type_to_string(shader_type::fragment))
                current_type = shader_type::fragment;
            else if (type == shader_utils::get_shader_type_to_string(shader_type::geometry))
                current_type = shader_type::geometry;

            shader_contents[current_type] = pos == std::string::npos
                                                ? shader_source.substr(next_line_pos)
                                                : shader_source.substr(next_line_pos, pos - next_line_pos);
        }

        LY_TRACE("Shader sources parsed successfully!");
        return shader_contents;
    }
}