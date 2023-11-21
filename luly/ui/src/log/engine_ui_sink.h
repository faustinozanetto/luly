#pragma once

#include "panels/console/console_panel.h"
#include "panels/console/console_panel_message.h"

#include "ui_api.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>

namespace luly::ui
{
    template <typename Mutex>
    class LULY_UI_API engine_ui_sink : public spdlog::sinks::base_sink<Mutex>
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


            const auto console_panel = console_panel::get_instance();
            console_panel->add_message(
                std::make_shared<console_panel_message>(fmt::to_string(formatted_buffer), source));
        }

        void flush_() override
        {
        }
    };
}
