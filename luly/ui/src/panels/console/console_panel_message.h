#pragma once

#include "ui_api.h"
#include <string>

namespace luly::ui
{
    class LULY_UI_API console_panel_message
    {
    public:
        console_panel_message(std::string message, std::string source) : m_message(std::move(message)),
                                                                         m_source(std::move(source))
        {
        }

        /* Getters */
        const std::string& get_message() const { return m_message; }
        const std::string& get_source() const { return m_source; }

    private:
        std::string m_message;
        std::string m_source;
    };
}
