#include "lypch.h"
#include "log.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace luly::core
{
    std::shared_ptr<spdlog::logger> log::s_logger_instance;
    std::vector<spdlog::sink_ptr> sinks;

    void log::initialize()
    {
        const auto& console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        const auto& file_sink = std::make_shared<
            spdlog::sinks::rotating_file_sink_mt>("logs/luly.log", 1048576 * 5, 3);

        console_sink->set_pattern("[%H:%M:%S %z] [%n] [thread %t] %v");
        /*
        console_sink->set_color(spdlog::level::trace, 37); // White
        console_sink->set_color(spdlog::level::debug, 36); // Cyan
        console_sink->set_color(spdlog::level::info, 32); // Green
        console_sink->set_color(spdlog::level::warn, 33); // Yellow
        console_sink->set_color(spdlog::level::err, 31); // Red
        console_sink->set_color(spdlog::level::critical, 1); // Bold Red
        */

        file_sink->set_pattern("[%H:%M:%S %z] [%n] [thread %t] %v");

        sinks.emplace_back(console_sink);
        sinks.emplace_back(file_sink);

        s_logger_instance = std::make_shared<spdlog::logger>("LULY", begin(sinks), end(sinks));
        spdlog::register_logger(s_logger_instance);
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_logger_instance->set_level(spdlog::level::trace);
    }

    void log::add_sink(spdlog::sink_ptr& sink)
    {
        s_logger_instance->sinks().push_back(sink);
        spdlog::set_pattern("%^[%T] %n: %v%$");
    }
}
