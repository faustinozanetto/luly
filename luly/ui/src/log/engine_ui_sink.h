#pragma once

#include "panels/console/console_panel.h"
#include "panels/console/console_panel_message.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <time.h>

namespace luly::ui
{
    template <typename Mutex>
    class engine_ui_sink : public spdlog::sinks::base_sink<Mutex>
    {
    public:
        explicit engine_ui_sink()
        {
        }

        engine_ui_sink(const engine_ui_sink&) = delete;
        engine_ui_sink& operator =(const engine_ui_sink&) = delete;
        virtual ~engine_ui_sink() = default;

        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted_buffer;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted_buffer);

            std::string source = fmt::format("File: {0} | Function: {1} | Line: {2}", msg.source.filename,
                                             msg.source.funcname, msg.source.line);

            // Convert time to time_t
            std::time_t timestamp = std::chrono::system_clock::to_time_t(msg.time);

            // Use localtime_r to get broken down time information
            std::tm tm;
            localtime_s(&tm, &timestamp);

            // Format timestamp in HOUR:MINUTE:SECONDS
            char timestamp_buffer[32];
            strftime(timestamp_buffer, sizeof(timestamp_buffer), "%H:%M:%S", &tm);

            const auto console_panel = console_panel::get_instance();
            console_panel->add_message(
                std::make_shared<console_panel_message>(msg.level, fmt::to_string(formatted_buffer), source, timestamp_buffer));
        }

        void flush_() override
        {
        }
    };
}
