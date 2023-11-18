project "luly-renderer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    
    targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs {
        "src",
        "%{_WORKING_DIR}/luly/core/src",
        "%{include_dirs.glfw}",
        "%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.assimp}",
        "%{include_dirs.stb}",
        "%{include_dirs.entt}",
    }
    
    links {
        "glfw",
        "glad",
        "stb",
        "luly-core"
    }
    
    filter "configurations:debug"
        defines "LY_DEBUG"
        runtime "Debug"
        symbols "on"
        links {
            "%{library_dirs.assimp_debug}"
        }
    
    filter "configurations:release"
        defines "LY_RELEASE;NDEBUG"
        runtime "Release"
        optimize "on"
        links {
            "%{library_dirs.assimp_release}"
        } 