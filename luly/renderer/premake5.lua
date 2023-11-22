project "luly-renderer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    defines { "LULY_RENDERER_EXPORTS" }
    
    targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs {
        "src",
        "%{_WORKING_DIR}/luly/shared/src",
        "%{include_dirs.glfw}",
        "%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.imgui}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.assimp}",
        "%{include_dirs.stb}",
    }
    
    links {
        "luly-shared",
        "glfw",
        "glad",
        "stb",
        "imgui"
    }
    
    filter "configurations:debug"
        defines "LY_DEBUG"
        runtime "Debug"
        symbols "on"
        links {
            "%{library_dirs.assimp_debug}",
        }
    
    filter "configurations:release"
        defines "LY_RELEASE;NDEBUG"
        runtime "Release"
        optimize "on"
        links {
            "%{library_dirs.assimp_release}"
        } 