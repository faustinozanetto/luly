#include "lypch.h"
#include "audio_emitter_component.h"

namespace luly::scene
{
    audio_emitter_component::audio_emitter_component(
        const std::shared_ptr<audio::audio_emitter>& audio_emitter)
    {
        m_audio_emitter = audio_emitter;
    }

    audio_emitter_component::~audio_emitter_component()
    {
    }

    void audio_emitter_component::initialize()
    {
    }
}
