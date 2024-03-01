project "luly-ui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    editandcontinue "Off"

    targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs {
        "src",
        "%{_WORKING_DIR}/luly/core/src",
        "%{_WORKING_DIR}/luly/shared/src",
        "%{include_dirs.glfw}",
        "%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.assimp}",
        "%{include_dirs.stb}",
        "%{include_dirs.entt}",
        "%{include_dirs.imgui}",
        "%{include_dirs.imguizmo}",
        "%{include_dirs.tracy}",
    }
    
    links {
        "luly-core",
        "luly-shared",
        "glfw",
        "glad",
        "stb",
        "imgui",
        "imguizmo",
    }
    
    filter "configurations:debug"
        defines {"LY_DEBUG", "TRACY_ENABLE", "TRACY_ON_DEMAND", "LY_PROFILING"}
        runtime "Debug"
        symbols "on"
    
    filter "configurations:release"
        defines "LY_RELEASE;NDEBUG"
        runtime "Release"
        optimize "on"