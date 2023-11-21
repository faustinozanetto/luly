#include "log.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace luly::shared
{
    std::shared_ptr<spdlog::logger> log::s_logger_instance;
    std::vector<spdlog::sink_ptr> sinks;

    void log::initialize()
    {
        const auto& console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        const auto& file_sink = std::make_shared<
            spdlog::sinks::rotating_file_sink_mt>("logs/luly.log", 1048576 * 5, 3);

        console_sink->set_pattern("[%H:%M:%S %z] [%n] [thread %t] %v");

        file_sink->set_pattern("[%H:%M:%S %z] [%n] [thread %t] %v");

        sinks.emplace_back(console_sink);
        sinks.emplace_back(file_sink);

        s_logger_instance = std::make_shared<spdlog::logger>("LULY", begin(sinks), end(sinks));
        spdlog::register_logger(s_logger_instance);
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_logger_instance->set_level(spdlog::level::trace);
    }

    void log::add_sink(const spdlog::sink_ptr& sink)
    {
        s_logger_instance->sinks().push_back(sink);
        spdlog::set_pattern("%^[%T] %n: %v%$");
    }
}
