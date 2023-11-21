﻿#pragma once

#include "texture.h"
#include "renderer_api.h"

#include <memory>
#include <string>

namespace luly::renderer
{
    class LULY_RENDERER_API texture_factory
    {
    public:
        static std::shared_ptr<texture> create_texture_from_file(const std::string& file_path);

    private:
        static texture_specification parse_texture_file_contents(const std::string& file_path);
    };
}
