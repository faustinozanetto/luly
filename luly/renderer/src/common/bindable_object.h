#pragma once

namespace luly::renderer
{
    class bindable_object
    {
    public:
        virtual ~bindable_object() = default;

        virtual void bind() = 0;
        virtual void un_bind() = 0;
    };
}
