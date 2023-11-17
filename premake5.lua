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
group ""

group "luly"
    include "luly/core"
    include "luly/renderer"
group ""

group "examples"
    include "examples/basic"
group ""