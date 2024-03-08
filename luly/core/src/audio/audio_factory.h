#pragma once

#include "audio_clip.h"

namespace luly::audio
{
    class audio_factory
    {
    public:
        static std::shared_ptr<audio_clip> create_audio_clip_from_file(const std::string& file_path);

    private:
        static audio_clip_data extract_audio_clip_data_from_wav(const std::string& file_path);
    };
}
