﻿#include "lypch.h"
#include "file_utils.h"

#include "application/application.h"

#include <commdlg.h>
#include <combaseapi.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace luly::utils
{
    std::string file_utils::open_file_dialog(const std::string& title, const std::vector<std::string>& filters)
    {
        std::string file_path;

        // Initialize COM library.
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        // Creating OPENFILENAME struct.
        OPENFILENAMEA ofn;
        ZeroMemory(&ofn, sizeof(ofn));
        char file_name[MAX_PATH] = "";
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window(core::application::get().get_window()->get_native_handle());
        ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
        ofn.lpstrFile = file_name;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = title.c_str();
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        // Append custom filters
        std::string filter;
        for (const std::string& f : filters)
        {
            filter += f + '\0';
            filter += f + '\0';
        }
        filter += '\0';
        ofn.lpstrFilter = filter.c_str();

        // Display the Open dialog
        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            file_path = file_name;
        }

        // Uninitialize COM library
        CoUninitialize();

        return file_path;
    }

    std::string file_utils::extract_file_name(const std::string& path)
    {
        const size_t slash_index = path.find_last_of("/\\");
        if (slash_index != std::string::npos)
        {
            return path.substr(slash_index + 1);
        }
        return path;
    }

    std::string file_utils::extract_file_extension(const std::string& path)
    {
        const size_t dot_index = path.find_last_of('.');
        if (dot_index != std::string::npos)
            return path.substr(dot_index);

        return "";
    }
}
