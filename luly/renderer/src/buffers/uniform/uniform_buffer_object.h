#pragma once

#include "renderer_api.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common/bindable_object.h"

namespace luly::renderer
{
    class LULY_RENDERER_API uniform_buffer_object : public bindable_object
    {
    public:
        uniform_buffer_object();
        ~uniform_buffer_object() override;

        /* Overrides */
        void bind(uint32_t slot) const;
        void bind() override;
        void un_bind() override;

        /* Getters */
        uint32_t get_handle_id() const { return m_handle_id; }

        /* Setters */
        void set_data(uint32_t size, void* data);

        /* Methods */
        void initialize(uint32_t size, void* data);

    private:
        uint32_t m_handle_id;
        uint8_t* m_data;
        uint32_t m_size;
    };
}
