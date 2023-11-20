#pragma once

#include <luly.h>

#include <arrays/vertex_array_object.h>
#include <shaders/shader.h>
#include <models/model.h>

#include "camera/perspective_camera.h"
#include "framebuffer/frame_buffer.h"
#include "textures/texture.h"

class basic_application : public luly::core::application
{
public:
    basic_application(const luly::renderer::window_specification& window_specification);
    ~basic_application() override;

    void on_update() override;
    void on_handle_event(luly::events::base_event& event) override;

private:
    void setup_fbo();
    void setup_camera();
    void setup_scene();

    std::shared_ptr<luly::renderer::shader> m_shader;
    std::shared_ptr<luly::renderer::model> m_model;
    std::shared_ptr<luly::renderer::texture> m_texture;
    std::shared_ptr<luly::renderer::frame_buffer> m_fbo;
    std::shared_ptr<luly::scene::scene_actor> m_actor;
    std::shared_ptr<luly::renderer::perspective_camera> m_camera;
};
