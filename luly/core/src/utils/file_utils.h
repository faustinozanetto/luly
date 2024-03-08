#pragma once

#include <string>
#include <vector>

namespace luly::utils
{
    class file_utils
    {
    public:
        static std::string open_file_dialog(const std::string& title, const std::vector<std::string>& filters);
        static std::string extract_file_name(const std::string& path);
        static std::string extract_file_extension(const std::string& path);
    };
}
