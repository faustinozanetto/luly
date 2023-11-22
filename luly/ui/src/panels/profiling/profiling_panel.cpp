﻿#include "profiling_panel.h"

#include <time/app_time.h>

namespace luly::ui
{
    bool profiling_panel::s_show = true;

    profiling_panel::profiling_panel() : ui_panel("profiling_panel")
    {
    }

    profiling_panel::~profiling_panel()
    {
    }

    void profiling_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Profiler", &s_show))
        {
            const auto frame_times = app_time::get_frame_times();
            // Calculate average frame time
            float average_frame_time = 0.0f;
            if (!frame_times.empty())
            {
                for (const float frame_time : frame_times)
                {
                    average_frame_time += frame_time;
                }
                average_frame_time /= frame_times.size();
            }

            ImGui::Text("FPS %.6f", app_time::get_fps());
            ImGui::Text("Raw Delta Time: %.6f ms", app_time::get_raw_delta_time());
            ImGui::Text("Delta Time: %.6f ms", app_time::get_delta_time());
            ImGui::Text("Frame Time: %.6f ms", app_time::get_frame_time());
            ImGui::Text("Time: %.4f s", app_time::get_time());
            ImGui::Text("Time Scale: %.2f ms", app_time::get_time_scale());

            char overlay[32];
            sprintf_s(overlay, "Avg %.6f ms", average_frame_time);

            float scale_min = 0.0f;
            float scale_max = 0.0f;

            for (const float frame_time : frame_times)
            {
                if (frame_time >= scale_max) scale_max = frame_time;
                if (frame_time < scale_min) scale_min = frame_time;
            }

            ImGui::PlotLines("Frame Times", frame_times.data(), static_cast<int>(app_time::get_frame_times().size()), 0,
                             overlay, -scale_max, scale_max, ImVec2(0, 80.0f));
            ImGui::End();
        }
    }

    bool profiling_panel::get_show_panel()
    {
        return s_show;
    }

    void profiling_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
