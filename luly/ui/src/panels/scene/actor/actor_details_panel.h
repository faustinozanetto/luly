#pragma once

#include "panels/ui_panel.h"

#include <vector>
#include <memory>

#include "components/actor_component_panel.h"

namespace luly::ui
{
    class actor_details_panel : public ui_panel
    {
    public:
        actor_details_panel();
        ~actor_details_panel() override;

        /* Overrides */
        void on_render_panel() override;

    private:
        void initialize_component_panels();
        
        std::vector<std::shared_ptr<actor_component_panel>> m_component_panels;
    };
}
