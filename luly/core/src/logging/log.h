#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace luly::core
{
    class log
    {
    public:
        static void initialize();

        /* Getters */
        static std::shared_ptr<spdlog::logger>& get_logger() { return s_logger_instance; }
        static void add_sink(spdlog::sink_ptr& sink);

    private:
        static std::shared_ptr<spdlog::logger> s_logger_instance;
    };
}

// Logging macros
#define LY_SEPARATOR(...) SPDLOG_LOGGER_CALL(::luly::core::log::get_logger(), spdlog::level::level_enum::trace, "==========================================================================================")
#define LY_INFO(...) SPDLOG_LOGGER_CALL(::luly::core::log::get_logger(), spdlog::level::level_enum::info, __VA_ARGS__)
#define LY_TRACE(...) SPDLOG_LOGGER_CALL(::luly::core::log::get_logger(), spdlog::level::level_enum::trace, __VA_ARGS__)
#define LY_WARN(...) SPDLOG_LOGGER_CALL(::luly::core::log::get_logger(), spdlog::level::level_enum::warn, __VA_ARGS__)
#define LY_ERROR(...) SPDLOG_LOGGER_CALL(::luly::core::log::get_logger(), spdlog::level::level_enum::err, __VA_ARGS__)
#define LY_CRITICAL(...) SPDLOG_LOGGER_CALL(::luly::core::log::get_logger(), spdlog::level::level_enum::critical, __VA_ARGS__)
