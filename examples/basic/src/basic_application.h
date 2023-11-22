#pragma once

#include <arrays/vertex_array_object.h>
#include <shaders/shader.h>
#include <models/model.h>
#include <events/key/key_pressed_event.h>
#include <framebuffer/frame_buffer.h>
#include <textures/texture.h>

#include <application/application.h>

class basic_application : public luly::core::application
{
public:
    basic_application(const luly::renderer::window_specification& window_specification);
    ~basic_application() override;

    /* Overrides */
    void on_create() override;
    void on_update() override;
    void on_handle_event(luly::events::base_event& event) override;

    /* Methods */
    bool on_key_pressed_event(const luly::events::key_pressed_event& key_pressed_event);

private:
    void setup_fbo();
    void setup_scene();

    std::shared_ptr<luly::renderer::shader> m_shader;
    std::shared_ptr<luly::renderer::model> m_model;
    std::shared_ptr<luly::renderer::texture> m_texture;
    std::shared_ptr<luly::renderer::frame_buffer> m_fbo;
    std::shared_ptr<luly::scene::scene_actor> m_actor;
};
