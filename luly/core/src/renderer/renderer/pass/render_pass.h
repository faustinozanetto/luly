﻿#pragma once

#include "renderer/framebuffer/frame_buffer.h"

#include <memory>
#include <string>
#include <unordered_map>

#include <logging/log.h>
#include <utils/profiler.h>

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
        render_pass(std::string name) : m_name(std::move(name))
        {
        }

        virtual ~render_pass() = default;

        /* Getters */
        const std::string& get_name() const { return m_name; }

        const render_pass_output& get_output(const std::string& name) const
        {
            LY_PROFILE_FUNCTION;
            return m_outputs.at(name);
        }

        const std::unordered_map<std::string, render_pass_output>& get_outputs() const
        {
            LY_PROFILE_FUNCTION;
            return m_outputs;
        }

        const std::shared_ptr<frame_buffer>& get_fbo() const
        {
            LY_PROFILE_FUNCTION;
            return m_fbo;
        }

        /* Virtuals */
        virtual void initialize() = 0;
        virtual void execute() = 0;
        virtual void set_outputs() = 0;
        virtual void on_resize(const glm::ivec2& dimensions) = 0;

        /* Methods */
        void add_input(const render_pass_input& render_pass_input)
        {
            LY_PROFILE_FUNCTION;
            m_inputs.insert({render_pass_input.name, render_pass_input});
        }

        void add_output(const render_pass_output& render_pass_output)
        {
            LY_PROFILE_FUNCTION;
            if (m_outputs.contains(render_pass_output.name))
            {
                LY_WARN("Replacing old output pass with name '{0}' to render pass with name '{1}'!",
                        render_pass_output.name, m_name);
            }

            m_outputs[render_pass_output.name] = render_pass_output;
        }

        void clear_outputs()
        {
            LY_PROFILE_FUNCTION;
            m_outputs.clear();
        }

    protected:
        std::string m_name;
        std::shared_ptr<frame_buffer> m_fbo;
        std::unordered_map<std::string, render_pass_input> m_inputs;
        std::unordered_map<std::string, render_pass_output> m_outputs;
    };
}
