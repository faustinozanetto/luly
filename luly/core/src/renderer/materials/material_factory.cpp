#include "lypch.h"
#include "material_factory.h"

#include "material_specification_builder.h"

namespace luly::renderer
{
    std::shared_ptr<material> material_factory::create_default_material()
    {
        LY_PROFILE_FUNCTION;
        const material_specification& material_specification = std::make_shared<material_specification_builder>()->
            build();
        const std::shared_ptr<material>& default_material = std::make_shared<material>(material_specification);

        return default_material;
    }
}
