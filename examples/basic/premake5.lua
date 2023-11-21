project "basic"
    kind "ConsoleApp"
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
        "%{_WORKING_DIR}/luly/renderer/src",
        "%{_WORKING_DIR}/luly/ui/src",
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
        "luly-core",
        "luly-renderer",
        "luly-ui",
        "luly-shared"
    }

    postbuildcommands {
        '{COPY} "%{_WORKING_DIR}/binaries/%{output_dir}/luly-core/luly-core.dll" "$(OutDir)"',
        '{COPY} "%{_WORKING_DIR}/binaries/%{output_dir}/luly-renderer/luly-renderer.dll" "$(OutDir)"',
        '{COPY} "%{_WORKING_DIR}/binaries/%{output_dir}/luly-shared/luly-shared.dll" "$(OutDir)"',
        '{COPY} "%{_WORKING_DIR}/binaries/%{output_dir}/luly-ui/luly-ui.dll" "$(OutDir)"',
    }
    
    postbuildmessage "Copying DLLs to $(OutDir)"
    
    filter "configurations:debug"
        defines "LY_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:release"
        defines "LY_RELEASE;NDEBUG"
        runtime "Release"
        optimize "on"