project "luly-core"
kind "SharedLib"
language "C++"
cppdialect "C++20"

targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")

pchheader "lypch.h"
pchsource "src/lypch.cpp"

editandcontinue "Off"

files {
    "src/**.h",
    "src/**.cpp",
}

includedirs
{
    "src",
    "%{include_dirs.glfw}",
    "%{include_dirs.glad}",
    "%{include_dirs.glm}",
    "%{include_dirs.spdlog}",
}

links
{
    "glfw",
    "glad",
}

filter "configurations:debug"
defines "LY_DEBUG"
runtime "Debug"
symbols "on"

links {
}

filter "configurations:release"
defines "LY_RELEASE;NDEBUG"
runtime "Release"
optimize "on"

links {
}