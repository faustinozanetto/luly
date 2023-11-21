#pragma once

#include "shader.h"
#include "renderer_api.h"

#include <memory>

namespace luly::renderer
{
    class LULY_RENDERER_API shader_factory
    {
    public:
        static std::shared_ptr<shader> create_shader_from_file(const std::string& file_path);

    private:
        static std::string read_shader_from_file(const std::string& file_path);
        static std::unordered_map<shader_type, std::string> parse_shader_source(const std::string& shader_source);
    };
}
