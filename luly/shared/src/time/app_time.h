#pragma once

#include "shared_api.h"

#include <chrono>
#include <vector>

namespace luly
{
    class LULY_SHARED_API app_time
    {
    public:
        using time_point = std::chrono::time_point<std::chrono::steady_clock>;
        using duration = std::chrono::high_resolution_clock::duration;

        static void set_delta_time(float t);
        static void set_time_scale(float t);

        static void update_time();

        static float get_raw_delta_time();
        static float get_delta_time();

        static float get_frame_time();
        static float get_fps();

        static float get_time();
        static std::vector<float>& get_frame_times();

        static float get_time_scale();

    private:
        static time_point s_now;
        static time_point s_previous_time;
        static duration s_delta_time;
        static std::vector<float> s_frame_times;
        static size_t s_ticks;
        static float s_time;
        static float s_time_scale;
    };
}
