#pragma once

#include <luly.h>

class basic_application : public luly::core::application
{
public:
    basic_application(const luly::renderer::window_specification& window_specification);
    ~basic_application() override;
};
