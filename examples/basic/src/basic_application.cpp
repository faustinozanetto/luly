#include "basic_application.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
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
