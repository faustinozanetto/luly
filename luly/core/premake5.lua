project "luly-core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    defines { "LULY_CORE_EXPORTS" }
    
    targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")
    
    pchheader "lypch.h"
    pchsource "src/lypch.cpp"
    
    files {
        "src/**.h",
        "src/**.cpp"
    }
    
    includedirs {
        "src",
        "%{_WORKING_DIR}/luly/renderer/src",
        "%{_WORKING_DIR}/luly/shared/src",
        "%{include_dirs.glfw}",
        "%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.assimp}",
        "%{include_dirs.entt}",
        "%{include_dirs.imgui}",
    }
    
    links {
        "luly-renderer",
        "luly-shared",
        "glfw",
        "glad",
        "imgui"
    }
    
    filter "configurations:debug"
        defines "LY_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:release"
        defines "LY_RELEASE;NDEBUG"
        runtime "Release"
        optimize "on"