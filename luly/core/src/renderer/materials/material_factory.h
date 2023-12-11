#pragma once

#include "material.h"

namespace luly::renderer {
    class material_factory {
        public:
			static std::shared_ptr<material> create_default_material();
    };
}