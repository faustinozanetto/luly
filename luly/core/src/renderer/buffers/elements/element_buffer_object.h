#pragma once

#include "renderer/common/bindable_object.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace luly::renderer
{
    class element_buffer_object : public bindable_object
    {
    public:
        element_buffer_object(int count = 0);
        ~element_buffer_object() override;

        /* Getters*/
        uint32_t get_handle_id() const { return m_handle; }
        int get_count() const { return m_count; }

        void bind() override;
        void un_bind() override;

        void set_data(int size, const void* data);

    private:
        uint32_t m_handle;
        int m_count;
    };
}
