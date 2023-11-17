#pragma once

#include <luly.h>

#include <arrays/vertex_array_object.h>
#include <shaders/shader.h>

class basic_application : public luly::core::application
{
public:
    basic_application(const luly::renderer::window_specification& window_specification);
    ~basic_application() override;

    void on_update() override;

private:
    void setup_triangle();

    std::shared_ptr<luly::renderer::vertex_array_object> m_triangle_vao;
    std::shared_ptr<luly::renderer::shader> m_shader;
};
