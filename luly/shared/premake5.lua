project "luly-shared"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    defines { "LULY_SHARED_EXPORTS" }
    targetextension ".dll"
    
    targetdir("%{_WORKING_DIR}/binaries/" .. output_dir .. "/%{prj.name}")
    objdir("%{_WORKING_DIR}/intermediates/" .. output_dir .. "/%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.cpp",
        "src/utils/assert.h",
        "src/input/key_codes.h",
        "src/input/mouse_codes.h",
    }
    
    includedirs {
        "src",
        "%{include_dirs.spdlog}",
        "%{include_dirs.glm}",
    }
    
    filter "configurations:debug"
        defines "LY_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:release"
        defines "LY_RELEASE;NDEBUG"
        runtime "Release"
        optimize "on"