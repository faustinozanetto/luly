#pragma once

#include "renderer/common/bindable_object.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace luly::renderer
{
    class uniform_buffer_object : public bindable_object
    {
    public:
        uniform_buffer_object(uint32_t size, uint32_t binding);
        ~uniform_buffer_object() override;

        /* Overrides */
        void bind(uint32_t slot) const;
        void bind() override;
        void un_bind() override;

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

        /* Setters */
        void set_data(const void* data, uint32_t size, uint32_t offset = 0);

    private:
        uint32_t m_handle_id;
    };
}
