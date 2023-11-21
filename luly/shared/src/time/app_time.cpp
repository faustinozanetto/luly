#include "app_time.h"

namespace luly
{
    app_time::time_point app_time::s_now = std::chrono::high_resolution_clock::now();
    app_time::time_point app_time::s_previous_time = std::chrono::high_resolution_clock::now();
    app_time::duration app_time::s_delta_time = duration::zero();
    size_t app_time::s_ticks = 0;
    float app_time::s_time = 0.0f;
    float app_time::s_time_scale = 1.0f;
    std::vector<float> app_time::s_frame_times;

    void app_time::set_delta_time(float t)
    {
        s_delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<float>(t));
    }

    void app_time::set_time_scale(float t)
    {
        s_time_scale = t;
    }

    void app_time::update_time()
    {
        // Calculate the frame time
        time_point current_time = std::chrono::high_resolution_clock::now();
        duration delta = current_time - s_previous_time;
        s_previous_time = current_time;
        float frame_time = std::chrono::duration_cast<std::chrono::duration<float>>(delta).count();

        // Store frame time for FPS calculation
        s_frame_times.push_back(frame_time);
        if (s_frame_times.size() > 200)
            s_frame_times.erase(s_frame_times.begin());

        s_ticks++;
        s_time += get_delta_time();
        s_delta_time = std::chrono::high_resolution_clock::now() - s_now;
        s_now = std::chrono::high_resolution_clock::now();
    }

    float app_time::get_raw_delta_time()
    {
        return s_delta_time.count() / 1000000000.0f;
    }

    float app_time::get_delta_time()
    {
        return get_raw_delta_time() * get_time_scale();
    }

    float app_time::get_frame_time()
    {
        if (s_frame_times.empty())
            return 0.0f;

        return s_frame_times.back();
    }

    float app_time::get_fps()
    {
        if (s_frame_times.empty())
            return 0.0f;

        float totalTime = 0.0f;
        for (float frame_time : s_frame_times)
            totalTime += frame_time;

        float avg_frame_time = totalTime / s_frame_times.size();
        if (avg_frame_time > 0.0f)
            return 1.0f / avg_frame_time;

        return 0.0f;
    }

    float app_time::get_time()
    {
        return s_time;
    }

    std::vector<float>& app_time::get_frame_times()
    {
        return s_frame_times;
    }

    float app_time::get_time_scale()
    {
        return s_time_scale;
    }
}
