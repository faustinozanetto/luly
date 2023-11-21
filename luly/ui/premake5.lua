project "luly-ui"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    defines { "LULY_UI_EXPORTS" }
    targetextension ".dll"
    
    targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs {
        "src",
        "%{_WORKING_DIR}/luly/core/src",
        "%{_WORKING_DIR}/luly/renderer/src",
        "%{_WORKING_DIR}/luly/shared/src",
        "%{include_dirs.glfw}",
        "%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.assimp}",
        "%{include_dirs.stb}",
        "%{include_dirs.entt}",
        "%{include_dirs.imgui}",
    }
    
    links {
        "luly-core",
        "luly-shared",
        "luly-renderer",
        "glfw",
        "glad",
        "stb",
        "imgui",
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