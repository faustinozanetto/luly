#pragma once

#include "renderer/framebuffer/frame_buffer.h"

#include <unordered_map>

namespace luly::renderer
{
    class render_pass;

    struct render_pass_input
    {
        std::shared_ptr<render_pass> render_pass;
        std::string name;
    };

    struct render_pass_output
    {
        uint32_t output;
        std::string name;
    };

    class render_pass
    {
    public:
        render_pass(const std::string& name);

        virtual ~render_pass() = default;

        /* Getters */
        const std::string& get_name() const { return m_name; }
        const render_pass_output& get_output(const std::string& name) const;
        const std::unordered_map<std::string, render_pass_output>& get_outputs() const;
        const std::shared_ptr<frame_buffer>& get_fbo() const;

        /* Virtuals */
        virtual void initialize() = 0;
        virtual void execute() = 0;
        virtual void set_outputs() = 0;
        virtual void on_resize(const glm::ivec2& dimensions) = 0;

        /* Methods */
        void add_input(const render_pass_input& render_pass_input);
        void add_output(const render_pass_output& render_pass_output);
        void clear_outputs();

    protected:
        std::string m_name;
        std::shared_ptr<frame_buffer> m_fbo;
        std::unordered_map<std::string, render_pass_input> m_inputs;
        std::unordered_map<std::string, render_pass_output> m_outputs;
    };
}
