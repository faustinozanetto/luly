#pragma once

#include "renderer_api.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace luly::renderer
{
    class LULY_RENDERER_API imgui_manager
    {
    public:
        imgui_manager(GLFWwindow* window);
        ~imgui_manager();

        /* Methods */
        void begin_frame();
        void end_frame();

    private:
        void initialize_imgui(GLFWwindow* window);

        void begin_dockspace();
        void end_dockspace();
    };
}
