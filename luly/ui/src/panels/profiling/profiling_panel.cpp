﻿#include "profiling_panel.h"

#include <time/app_time.h>

#include "utils/ui_utils.h"

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

            ImGui::Text("FPS %.6f", app_time::get_fps());
            ImGui::Text("Raw Delta Time: %.6f ms", app_time::get_raw_delta_time());
            ImGui::Text("Delta Time: %.6f ms", app_time::get_delta_time());
            ImGui::Text("Frame Time: %.6f ms", app_time::get_frame_time());
            ImGui::Text("Time: %.4f s", app_time::get_time());

            float time_scale = app_time::get_time_scale();
            if (ui_utils::draw_property("Time Scale", time_scale, 0.01f, 5.0f, 0.001f))
            {
                app_time::set_time_scale(time_scale);
            }

            ImGui::PlotLines("Frame Times", frame_times.data(), static_cast<int>(app_time::get_frame_times().size()), 0,
                             nullptr, -0.5f, 0.5f, ImVec2(0, 80.0f));
        }
        ImGui::End();
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
