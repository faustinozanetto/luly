#include "lypch.h"
#include "shader_factory.h"

#include "shader_utils.h"

#include <fstream>

#define SHADER_TYPE_TOKEN "#shader"
#define SHADER_INCLUDE_TOKEN "#include"

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

        while (pos != std::string::npos)
        {
            const size_t eol = shader_source.find_first_of("\r\n", pos);
            const size_t begin = pos + shader_type_token_len + 1;
            std::string type = shader_source.substr(begin, eol - begin);

            const size_t next_line_pos = shader_source.find_first_not_of("\r\n", eol);
            pos = shader_source.find(SHADER_TYPE_TOKEN, next_line_pos);

            auto current_type = shader_type::fragment;

            if (type == shader_utils::get_shader_type_to_string(shader_type::vertex))
                current_type = shader_type::vertex;
            else if (type == shader_utils::get_shader_type_to_string(shader_type::fragment))
                current_type = shader_type::fragment;
            else if (type == shader_utils::get_shader_type_to_string(shader_type::geometry))
                current_type = shader_type::geometry;

            std::unordered_set<std::string> included_files;
            std::stringstream updated_source;
            std::istringstream source_stream(pos == std::string::npos
                                                 ? shader_source.substr(next_line_pos)
                                                 : shader_source.substr(next_line_pos, pos - next_line_pos));
            std::string line;

            while (std::getline(source_stream, line))
            {
                if (line.find(SHADER_INCLUDE_TOKEN) != std::string::npos)
                {
                    std::istringstream include_stream(line);
                    std::string directive, include_path;
                    include_stream >> directive >> include_path;

                    // Remove quotation marks from the include path
                    std::erase(include_path, '\"');

                    // Check if the file is not already included to avoid infinite recursion
                    if (!included_files.contains(include_path))
                    {
                        included_files.insert(include_path);
                        // Recursively parse the included file
                        std::string include_code = parse_shader_includes(include_path, included_files);
                        updated_source << include_code << std::endl;
                    }
                    else
                    {
                        LY_ERROR("Circular dependency detected in shader file: {0}!", include_path);
                    }
                }
                else
                {
                    updated_source << line << std::endl;
                }
            }
            // Store the parsed source.
            shader_contents[current_type] = updated_source.str();
        }

        LY_TRACE("Shader sources parsed successfully!");
        return shader_contents;
    }

    std::string shader_factory::parse_shader_includes(const std::string& file_path,
                                                      std::unordered_set<std::string>& included_files)
    {
        LY_TRACE("Parsing shader include file: {}", file_path);

        std::string full_path = "assets/shaders/" + file_path;
        std::ifstream file_stream(full_path);
        if (!file_stream.is_open())
        {
            LY_ERROR("Failed to open shader include file: {0}!", file_path);
            LY_ASSERT(false);
        }

        std::stringstream include_code;
        std::string line;

        while (std::getline(file_stream, line))
        {
            if (line.find("#include") != std::string::npos)
            {
                std::istringstream include_stream(line);
                std::string directive, include_path;
                include_stream >> directive >> include_path;

                // Remove quotation marks from the include path
                std::erase(include_path, '\"');

                // Check if the file is not already included to avoid infinite recursion
                if (!included_files.contains(include_path))
                {
                    included_files.insert(include_path);
                    // Recursively parse the included file
                    std::string includedCode = parse_shader_includes(include_path, included_files);
                    include_code << includedCode << std::endl;
                }
                else
                {
                    std::cerr << "Warning: Circular include detected in file " << file_path << " for " << include_path
                        <<
                        std::endl;
                }
            }
            else
            {
                include_code << line << std::endl;
            }
        }

        LY_TRACE("Shader file parsed successfully: {}", file_path);
        return include_code.str();
    }
}
