#include "lypch.h"
#include "audio_factory.h"

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

#include "utils/file_utils.h"

namespace luly::audio
{
    std::shared_ptr<audio_clip> audio_factory::create_audio_clip_from_file(const std::string& file_path)
    {
        LY_TRACE("Started loading audio clip from file...");
        audio_clip_data data;
        const std::string& file_extension = utils::file_utils::extract_file_extension(file_path);

        if (file_extension == ".wav")
        {
            data = extract_audio_clip_data_from_wav(file_path);
        }
        else
        {
            LY_ASSERT_MSG(false, "Invalid audio clip extension!")
        }

        std::shared_ptr<audio_clip> audio_clip = std::make_shared<class audio_clip>(data);
        LY_TRACE("Audio clip loaded from file successfully!");

        return audio_clip;
    }

    audio_clip_data audio_factory::extract_audio_clip_data_from_wav(const std::string& file_path)
    {
        audio_clip_data audio_clip_data;

        drwav wav;
        const bool init_result = drwav_init_file(&wav, file_path.c_str(), nullptr);
        LY_ASSERT_MSG(init_result, "Could not open audio clip file: {0}", m_file_path.c_str());

        audio_clip_data.frequency = static_cast<float>(wav.sampleRate);
        audio_clip_data.length = static_cast<float>(wav.totalPCMFrameCount) / wav.sampleRate;
        audio_clip_data.bit_rate = wav.bitsPerSample;
        audio_clip_data.channels = wav.channels;
        audio_clip_data.size = static_cast<int>(wav.totalPCMFrameCount * wav.channels * (wav.bitsPerSample / 8));
        audio_clip_data.data = new unsigned char[audio_clip_data.size];

        drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, audio_clip_data.data);

        // Clean up the WAV file
        drwav_uninit(&wav);

        return audio_clip_data;
    }
}
