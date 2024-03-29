﻿#pragma once

#include <events/key/key_pressed_event.h>

#include <application/application.h>

#include "engine_ui.h"

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
    void setup_scene();
    void create_ball(const std::shared_ptr<luly::scene::scene>& scene, float radius = 0.5f, float impulse = 10.0f);
    
    std::shared_ptr<luly::ui::engine_ui> m_engine_ui;
};
