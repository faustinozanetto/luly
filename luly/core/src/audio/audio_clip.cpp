#include "lypch.h"
#include "audio_clip.h"

#include <AL/al.h>

namespace luly::audio
{
    audio_clip::audio_clip(const audio_clip_data& audio_clip_data)
    {
        m_data = audio_clip_data;
        LY_TRACE("Started creating audio clip...");
        LY_TRACE("Audio Clip Information:");
        LY_TRACE("    - Frequency: {0}", m_data.frequency);
        LY_TRACE("    - Length: {0}", m_data.length);
        LY_TRACE("    - Bit Rate: {0}", m_data.bit_rate);
        LY_TRACE("    - Size: {0}", m_data.size);
        LY_TRACE("    - Channels: {0}", m_data.channels);

        // Generate OpenAL buffers
        alGenBuffers(1, &m_audio_buffer);
        alBufferData(m_audio_buffer, AL_FORMAT_STEREO16, m_data.data,
                     m_data.size,
                     static_cast<ALsizei>(m_data.frequency));
        LY_TRACE("Audio clip created successfully!");
    }

    audio_clip::~audio_clip()
    {
        alDeleteBuffers(1, &m_audio_buffer);
    }
}
