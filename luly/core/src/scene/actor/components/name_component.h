#pragma once

#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class name_component : public base_component
    {
    public:
        name_component(const std::string& name) : m_name(name)
        {
        }

        ~name_component() override
        {
        };

        /* Getters */
        const std::string& get_name() const { return m_name; }

        /* Setters */
        void set_name(const std::string& name) { m_name = name; }

        void initialize() override
        {
        }

    private:
        std::string m_name;
    };
}
