#include "lypch.h"
#include "audio_emitter.h"

namespace luly::audio
{
    audio_emitter::audio_emitter()
    {
        m_location = glm::vec3(0.0f, 0.0f, 0.0f);
        m_pitch = 1.0f;
        m_volume = 1.0f;
        m_range = 50.0f;
        m_loop = false;
        m_paused = false;
        alGenSources(1, &m_source);
    }

    audio_emitter::~audio_emitter()
    {
        alDeleteSources(1, &m_source);
    }

    void audio_emitter::set_location(const glm::vec3& location)
    {
        m_location = location;
        alSourcefv(m_source, AL_POSITION, reinterpret_cast<float*>(&m_location));
    }

    void audio_emitter::set_audio_clip(const std::shared_ptr<audio_clip>& audio_clip)
    {
        stop();
        m_audio_clip = audio_clip;
        alSourcei(m_source, AL_BUFFER, m_audio_clip->get_audio_buffer());
        alSourcef(m_source, AL_MAX_DISTANCE, m_range);
        alSourcef(m_source, AL_REFERENCE_DISTANCE, 1.0f);
        alSourcef(m_source, AL_ROLLOFF_FACTOR, 1.0f);
        alSourcei(m_source, AL_LOOPING, m_loop ? 1 : 0);
        alSourcef(m_source, AL_GAIN, m_volume);
        alSourcef(m_source, AL_PITCH, m_pitch);
    }

    void audio_emitter::set_volume(float volume)
    {
        m_volume = volume;
        alSourcef(m_source, AL_GAIN, m_volume);
    }

    void audio_emitter::set_pitch(float pitch)
    {
        m_pitch = pitch;
        alSourcef(m_source, AL_PITCH, m_pitch);
    }

    void audio_emitter::set_range(float range)
    {
        m_range = range;
        alSourcef(m_source, AL_MAX_DISTANCE, m_range);
    }

    void audio_emitter::set_is_loop(bool is_loop)
    {
        m_loop = is_loop;
        alSourcei(m_source, AL_LOOPING, m_loop ? 1 : 0);
    }

    void audio_emitter::play()
    {
        alSourcePlay(m_source);
        m_paused = false;
    }

    void audio_emitter::pause()
    {
        alSourcePause(m_source);
        m_paused = true;
    }

    void audio_emitter::stop()
    {
        alSourceStop(m_source);
        m_paused = false;
    }
}
