#pragma once

#include "texture.h"
#include "texture_2d.h"
#include "texture_cubemap.h"

#include <memory>
#include <string>

namespace luly::renderer
{
    class texture_factory
    {
    public:
        static std::shared_ptr<texture_2d> create_texture_from_file(const std::string& file_path);
        static std::shared_ptr<texture_2d> create_environment_texture_from_file(const std::string& file_path);
        static std::shared_ptr<texture_cubemap> create_texture_cubemap_from_file(const std::string& file_path);

    private:
        static texture_specification parse_texture_file_contents(const std::string& file_path);
        static texture_specification parse_environment_texture_file_contents(const std::string& file_path);
    };
}
