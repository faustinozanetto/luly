﻿#pragma once

#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class identifier_component : public base_component
    {
    public:
        identifier_component(const utils::uuid& uuid = utils::uuid()) : m_uuid(uuid)
        {
        }

        ~identifier_component() override
        {
        };

        /* Getters */
        const utils::uuid& get_identifier() const { return m_uuid; }

        void initialize() override
        {
        }

    private:
        utils::uuid m_uuid;
    };
}
