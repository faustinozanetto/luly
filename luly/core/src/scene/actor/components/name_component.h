#pragma once

#include "scene/actor/components/base_component.h"

#include "core_api.h"

namespace luly::scene
{
    class LULY_CORE_API name_component : public base_component
    {
    public:
        name_component(const std::string& name);

        ~name_component() override;

        void initialize() override;

        /* Getters */
        const std::string& get_name() const { return m_name; }

        /* Setters */
        void set_name(const std::string& name) { m_name = name; }

    private:
        std::string m_name;
    };
}
