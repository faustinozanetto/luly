#pragma once

namespace luly::audio
{
    struct audio_clip_data
    {
        float frequency;
        float length;
        uint32_t bit_rate;
        uint32_t channels;
        int size;
        void* data;
    };

    class audio_clip
    {
    public:
        audio_clip(const audio_clip_data& audio_clip_data);
        ~audio_clip();

        /* Getters */
        float get_frequency() const { return m_data.frequency; }
        float get_length() const { return m_data.length; }
        uint32_t get_bit_rate() const { return m_data.bit_rate; }
        uint32_t get_channels() const { return m_data.channels; }
        uint32_t get_audio_buffer() const { return m_audio_buffer; }

    private:
        audio_clip_data m_data;
        uint32_t m_audio_buffer;
    };
}
