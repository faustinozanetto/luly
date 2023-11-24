#pragma once

#include "renderer/framebuffer/frame_buffer.h"

#include <memory>
#include <string>

namespace luly::renderer
{
    class render_pass;

    struct render_pass_input
    {
        std::shared_ptr<render_pass> render_pass;
        std::string name;
    };

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
        virtual void execute() = 0;

        /* Methods */
        void add_input(const render_pass_input& render_pass_input)
        {
            m_inputs.insert({render_pass_input.name, render_pass_input});
        }

    protected:
        std::string m_name;
        std::shared_ptr<frame_buffer> m_fbo;
        std::unordered_map<std::string, render_pass_input> m_inputs;
    };
}
