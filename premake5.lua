include "dependencies.lua"

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "luly"
architecture "x86_64"

configurations
{
    "debug",
    "release"
}

flags
{
    "MultiProcessorCompile"
}

group "dependencies"
    include "third_party/glfw"
    include "third_party/glad"
    include "third_party/stb"
    include "third_party/imgui"
    include "third_party/imguizmo"
group ""

group "luly"
    include "luly/core"
    include "luly/ui"
    include "luly/shared"
group ""

group "examples"
    include "examples/basic"
group ""