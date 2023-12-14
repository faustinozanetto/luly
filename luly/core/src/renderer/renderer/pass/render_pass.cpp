#include "lypch.h"
#include "render_pass.h"

namespace luly::renderer
{
    render_pass::render_pass(const std::string& name)
    {
        m_name = name;
    }

    const render_pass_output& render_pass::get_output(const std::string& name) const
    {
        LY_PROFILE_FUNCTION;
        return m_outputs.at(name);
    }

    const std::unordered_map<std::string, render_pass_output>& render_pass::get_outputs() const
    {
        LY_PROFILE_FUNCTION;
        return m_outputs;
    }

    const std::shared_ptr<frame_buffer>& render_pass::get_fbo() const
    {
        LY_PROFILE_FUNCTION;
        return m_fbo;
    }

    void render_pass::add_input(const render_pass_input& render_pass_input)
    {
        LY_PROFILE_FUNCTION;
        m_inputs.insert({render_pass_input.name, render_pass_input});
    }

    void render_pass::add_output(const render_pass_output& render_pass_output)
    {
        LY_PROFILE_FUNCTION;
        if (m_outputs.contains(render_pass_output.name))
        {
            LY_WARN("Replacing old output pass with name '{0}' to render pass with name '{1}'!",
                    render_pass_output.name, m_name);
        }

        m_outputs[render_pass_output.name] = render_pass_output;
    }

    void render_pass::clear_outputs()
    {
        LY_PROFILE_FUNCTION;
        m_outputs.clear();
    }
}
