#include "lypch.h"
#include "name_component.h"

namespace luly::scene
{
    name_component::name_component(const std::string& name)
    {
        m_name = name;
    }

    name_component::~name_component()
    {
    }

    void name_component::initialize()
    {
    }
}
