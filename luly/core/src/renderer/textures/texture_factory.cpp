#include "lypch.h"
#include "texture_factory.h"

#include <logging/log.h>
#include <utils/assert.h>

#include <stb_image.h>

namespace luly::renderer
{
    std::shared_ptr<texture_2d> texture_factory::create_texture_from_file(const std::string& file_path)
    {
        LY_TRACE("Started loading texture from file...");
        const texture_specification& texture_specification = parse_texture_file_contents(file_path);
        auto created_texture = std::make_shared<texture_2d>(texture_specification);
        LY_TRACE("Texture loaded from file successfully!");
        return created_texture;
    }

    std::shared_ptr<texture_2d> texture_factory::create_environment_texture_from_file(const std::string& file_path)
    {
        LY_TRACE("Started loading environment texture from file...");
        const texture_specification& texture_specification = parse_texture_file_contents(file_path);
        auto created_texture = std::make_shared<texture_2d>(texture_specification);
        LY_TRACE("Environment texture loaded from file successfully!");
        return created_texture;
    }

    std::shared_ptr<texture_cubemap> texture_factory::create_texture_cubemap_from_file(const std::string& file_path)
    {
        LY_TRACE("Started loading cubemap texture from file...");
        const texture_specification& texture_specification = parse_environment_texture_file_contents(file_path);
        auto created_texture = std::make_shared<texture_cubemap>(texture_specification);
        LY_TRACE("Cubemap texture loaded from file successfully!");
        return created_texture;
    }

    texture_specification texture_factory::parse_texture_file_contents(const std::string& file_path)
    {
        LY_TRACE("Started parsing texture file...");
        LY_TRACE("  - File Path: '{0}'", file_path);

        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        uint8_t* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);

        LY_ASSERT_MSG(data != nullptr, "Could not read data from texture image file!")
        LY_ASSERT_MSG(channels > 0, "Invalid texture channels count!")

        LY_TRACE("Texture file parsed successfully.");

        return texture_specification{width, height, channels, data};
    }

    texture_specification texture_factory::parse_environment_texture_file_contents(const std::string& file_path)
    {
        LY_TRACE("Started parsing environment texture file...");
        LY_TRACE("  - File Path: '{0}'", file_path);

        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        uint8_t* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);

        LY_ASSERT_MSG(data != nullptr, "Could not read data from environemnt texture image file!")
        LY_ASSERT_MSG(channels > 0, "Invalid texture channels count!")

        LY_TRACE("Environment texture file parsed successfully.");

        return texture_specification{width, height, channels, data, texture_internal_format::rgba16f};
    }
}
