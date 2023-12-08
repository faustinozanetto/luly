#pragma once

#include <string>
#include <spdlog/common.h>

namespace luly::ui
{
    class console_panel_message
    {
    public:
        console_panel_message(spdlog::level::level_enum level, std::string message, std::string source, std::string time) : m_level(level),
            m_message(std::move(message)),
            m_time(std::move(time)), m_source(std::move(source))
        {
        }

        /* Getters */
        spdlog::level::level_enum get_level() const { return m_level; }
        const std::string& get_message() const { return m_message; }
        const std::string& get_source() const { return m_source; }
        const std::string& get_time() const { return m_time; }

    private:
        spdlog::level::level_enum m_level;
        std::string m_message;
        std::string m_time;
        std::string m_source;
    };
}
