#pragma once

#include "audio/audio_emitter.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class audio_emitter_component : public base_component
    {
    public:
        audio_emitter_component(const std::shared_ptr<audio::audio_emitter>& audio_emitter);
        ~audio_emitter_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<audio::audio_emitter>& get_audio_emitter() const
        {
            return m_audio_emitter;
        }

        /* Setters */
        void set_audio_emitter(const std::shared_ptr<audio::audio_emitter>& audio_emitter)
        {
            m_audio_emitter = audio_emitter;
        }

    private:
        std::shared_ptr<audio::audio_emitter> m_audio_emitter;
    };
}
