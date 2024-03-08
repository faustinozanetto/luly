#pragma once

#include "audio_clip.h"

#include <AL/al.h>
#include <glm/glm.hpp>

namespace luly::audio
{
    class audio_emitter
    {
    public:
        audio_emitter();
        ~audio_emitter();

        /* Getters */
        glm::vec3 get_location() const { return m_location; }
        const std::shared_ptr<audio_clip>& get_audio_clip() const { return m_audio_clip; }
        float get_volume() const { return m_volume; }
        float get_pitch() const { return m_pitch; }
        float get_range() const { return m_range; }
        bool get_is_loop() const { return m_loop; }
        bool get_is_paused() const { return m_paused; }

        /* Setters */
        void set_location(const glm::vec3& location);
        void set_audio_clip(const std::shared_ptr<audio_clip>& audio_clip);
        void set_volume(float volume);
        void set_pitch(float pitch);
        void set_range(float range);
        void set_is_loop(bool is_loop);

        /* Methods */
        void play();
        void pause();
        void stop();

    private:
        ALuint m_source;
        std::shared_ptr<audio_clip> m_audio_clip;
        glm::vec3 m_location;

        float m_volume;
        float m_pitch;
        float m_range;
        bool m_loop;
        bool m_paused;
    };
}
