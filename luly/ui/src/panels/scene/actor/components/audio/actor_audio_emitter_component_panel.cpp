#include "actor_audio_emitter_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/audio/audio_emitter_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_audio_emitter_component_panel::actor_audio_emitter_component_panel() : actor_component_panel(
        "Audio Emitter Component")
    {
    }

    std::pair<bool, size_t> actor_audio_emitter_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<
            scene::audio_emitter_component>();
        auto component_hash = typeid(scene::audio_emitter_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_audio_emitter_component_panel::on_render_component_details()
    {
        const scene::audio_emitter_component& audio_emitter_component = engine_ui::get().
                                                                        get_selected_actor()->
                                                                        get_component<
                                                                            scene::audio_emitter_component>();

        const std::shared_ptr<audio::audio_emitter>& audio_emitter = audio_emitter_component.
            get_audio_emitter();

        ui_utils::draw_property("Emitter Controls");

        if (ui_utils::draw_button("Play"))
        {
            audio_emitter->play();
        }
        if (ui_utils::draw_button("Stop"))
        {
            audio_emitter->stop();
        }
        if (ui_utils::draw_button("Pause"))
        {
            audio_emitter->pause();
        }

        ImGui::Separator();

        float volume = audio_emitter->get_volume();
        if (ui_utils::draw_property("Volume", volume, 0.0f, 5.0f, 0.01f))
        {
            audio_emitter->set_volume(volume);
        }

        float pitch = audio_emitter->get_pitch();
        if (ui_utils::draw_property("Pitch", pitch, 0.0f, 5.0f, 0.01f))
        {
            audio_emitter->set_pitch(pitch);
        }

        bool is_loop = audio_emitter->get_is_loop();
        if (ui_utils::draw_property("Is Loop", is_loop))
        {
            audio_emitter->set_is_loop(is_loop);
        }
    }
}
