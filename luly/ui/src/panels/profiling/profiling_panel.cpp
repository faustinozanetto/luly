#include "profiling_panel.h"

#include "time/engine_time.h"

namespace luly::ui
{
    profiling_panel::profiling_panel() : ui_panel("profiling_panel")
    {
    }

    profiling_panel::~profiling_panel()
    {
    }

    void profiling_panel::on_render_panel()
    {
        const auto frame_times = time::get_frame_times();
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

        ImGui::Begin("Profiler", &m_show);
        ImGui::Text("FPS %.6f", time::get_fps());
        ImGui::Text("Raw Delta Time: %.6f ms", time::get_raw_delta_time());
        ImGui::Text("Delta Time: %.6f ms", time::get_delta_time());
        ImGui::Text("Frame Time: %.6f ms", time::get_frame_time());
        ImGui::Text("Time: %.4f s", time::get_time());
        ImGui::Text("Time Scale: %.2f ms", time::get_time_scale());

        char overlay[32];
        sprintf_s(overlay, "Avg %.6f ms", average_frame_time);
        ImGui::PlotLines("Frame Times", frame_times.data(), static_cast<int>(time::get_frame_times().size()), 0,
                         overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
        ImGui::End();
    }
}
