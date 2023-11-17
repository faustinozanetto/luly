#include "basic_application.h"

#include <application/entry_point.h>

#include "shaders/shader.h"
#include "shaders/shader_factory.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    std::shared_ptr<luly::renderer::shader> shader = luly::renderer::shader_factory::create_shader_from_file("assets/shaders/test_shader.lsh");
}

basic_application::~basic_application()
{
}

luly::core::application* luly::core::create_application()
{
    renderer::window_specification window_specification = {
        "Basic Application", 1280, 720
    };
    return new basic_application(window_specification);
}
