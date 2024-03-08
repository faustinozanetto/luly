project "basic"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

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
        "%{_WORKING_DIR}/luly/ui/src",
        "%{_WORKING_DIR}/luly/shared/src",
        "%{include_dirs.glfw}",
        "%{include_dirs.glad}",
        "%{include_dirs.glm}",
        "%{include_dirs.spdlog}",
        "%{include_dirs.assimp}",
        "%{include_dirs.entt}",
        "%{include_dirs.imgui}",
        "%{include_dirs.imguizmo}",
        "%{include_dirs.tracy}",
        "%{include_dirs.physx}",
        "%{include_dirs.physx_omnipvd}",
        "%{include_dirs.yaml_cpp}",
        "%{include_dirs.openal}",
    }
    
    links {
        "luly-core",
        "luly-ui",
        "luly-shared",
        "glfw",
        "glad",
        "stb",
        "yaml-cpp"
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:debug"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:release"
        defines "NDEBUG"
        runtime "Release"
        optimize "on"