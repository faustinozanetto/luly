#include "space_shooter_application.h"

#include <application/entry_point.h>
#include <events/event_dispatcher.h>

#include "scene/game_scene.h"

space_shooter_application::space_shooter_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    m_engine_ui = std::make_shared<luly::ui::engine_ui>();
    space_shooter_application::on_create();
}

space_shooter_application::~space_shooter_application()
{
}

void space_shooter_application::on_create()
{
    const std::shared_ptr<game_scene>& space_game_scene = std::make_shared<game_scene>();
    luly::scene::scene_manager::get().add_scene(space_game_scene);
    luly::scene::scene_manager::get().switch_scene(space_game_scene);
}

void space_shooter_application::on_update()
{
    application::on_update();

    m_engine_ui->begin_frame();

    m_engine_ui->on_update();
    m_engine_ui->end_frame();
}

void space_shooter_application::on_handle_event(luly::events::base_event& event)
{
    m_engine_ui->on_event(event);
}

luly::core::application* luly::core::create_application()
{
    std::filesystem::current_path("../../");
    const renderer::window_specification window_specification = {
        "Space Shooter", 1080, 1080
    };
    return new space_shooter_application(window_specification);
}
