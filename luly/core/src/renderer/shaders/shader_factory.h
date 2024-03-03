#pragma once

#include "shader.h"

#include <memory>
#include <unordered_set>

namespace luly::renderer
{
    class shader_factory
    {
    public:
        static std::shared_ptr<shader> create_shader_from_file(const std::string& file_path);
        static std::unordered_map<shader_type, std::string> extract_shader_contents(const std::string& file_path);
        static std::string read_shader_from_file(const std::string& file_path);

    private:
        static std::vector<std::string> extract_shader_source_lines(const std::string& shader_source);
        static std::unordered_map<shader_type, std::string> parse_shader_source(
            const std::string& shader_source, std::unordered_set<std::string>& include_files);
        static std::string parse_shader_includes(const std::string& file_path,
                                                 std::unordered_set<std::string>& include_files);
    };
}
