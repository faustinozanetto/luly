project "luly/core"
kind "SharedLib"
language "C++"
cppdialect "C++20"

targetdir("%{wks.location}/binaries/" .. output_dir .. "/%{prj.name}")
objdir("%{wks.location}/intermediates/" .. output_dir .. "/%{prj.name}")

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