#pragma once

#include "renderer/framebuffer/frame_buffer.h"

#include <memory>
#include <string>

namespace luly::renderer
{
    class render_pass
    {
    public:
        render_pass(std::string name) : m_name(std::move(name))
        {
        }

        virtual ~render_pass() = default;

        /* Getters */
        const std::string& get_name() const { return m_name; }
        const std::shared_ptr<frame_buffer>& get_frame_buffer() const { return m_fbo; }

        /* Virtuals */
        virtual void initialize() = 0;

    protected:
        std::string m_name;
        std::shared_ptr<frame_buffer> m_fbo;
    };
}
