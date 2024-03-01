#pragma once

#include <application/application.h>
#include <engine_ui.h>

class space_shooter_application : public luly::core::application
{
public:
    space_shooter_application(const luly::renderer::window_specification& window_specification);
    ~space_shooter_application() override;

    /* Overrides */
    void on_create() override;
    void on_update() override;
    void on_handle_event(luly::events::base_event& event) override;

private:
    std::shared_ptr<luly::ui::engine_ui> m_engine_ui;
};
