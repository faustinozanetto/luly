project "luly-core"
kind "StaticLib"
language "C++"
cppdialect "C++20"

editandcontinue "Off"

targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")

pchheader "lypch.h"
pchsource "src/lypch.cpp"

files {
    "src/**.h",
    "src/**.cpp",
    "%{wks.location}/third_party/tracy/public/TracyClient.cpp"
}

includedirs {
    "src",
    "%{_WORKING_DIR}/luly/shared/src",
    "%{include_dirs.glfw}",
    "%{include_dirs.glad}",
    "%{include_dirs.glm}",
    "%{include_dirs.spdlog}",
    "%{include_dirs.assimp}",
    "%{include_dirs.entt}",
    "%{include_dirs.stb}",
    "%{include_dirs.tracy}",
    "%{include_dirs.physx}",
    "%{include_dirs.physx_omnipvd}",
    "%{include_dirs.yaml_cpp}",
    "%{include_dirs.openal}",
    "%{include_dirs.dr_libs}",
}

links {
    "luly-shared",
    "glfw",
    "glad",
    "yaml-cpp",
    "stb",
}

filter "configurations:debug"
defines { "LY_DEBUG", "TRACY_ENABLE", "TRACY_ON_DEMAND", "LY_PROFILING" }
runtime "Debug"
symbols "on"
links {
    "%{library_dirs.assimp_debug}",
    "%{library_dirs.openal_debug}",
    "%{library_dirs.physx_debug}",
    "%{library_dirs.physx_common_debug}",
    "%{library_dirs.physx_foundation_debug}",
    "%{library_dirs.physx_extensions_debug}",
    "%{library_dirs.physx_pvd_runtime_debug}",
    "%{library_dirs.physx_pvd_sdk_debug}",
}

filter "configurations:release"
defines "LY_RELEASE;NDEBUG"
runtime "Release"
optimize "on"
links {
    "%{library_dirs.assimp_release}",
    "%{library_dirs.openal_release}",
    "%{library_dirs.physx_release}",
    "%{library_dirs.physx_common_release}",
    "%{library_dirs.physx_foundation_release}",
    "%{library_dirs.physx_extensions_release}",
    "%{library_dirs.physx_pvd_runtime_release}",
    "%{library_dirs.physx_pvd_sdk_release}",
} 