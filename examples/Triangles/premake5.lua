project "Triangles"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "Off"

  location "build/%{prj.name}"

  targetdir "bin/%{cfg.buildcfg}"
  objdir "build/%{cfg.buildcfg}"

  dependson "Andromeda"

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  }

  files
  {
    "src/**.cpp",
    "src/**.h",
    "premake5.lua"
  }

  includedirs
  {
    "../../include",
  }

  links
  {
    "../../bin/%{cfg.buildcfg}/Andromeda",
  }

  filter "configurations:Debug"
    defines { "DEBUG" }
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    defines { "NDEBUG" }
    runtime "Release"
    optimize "on"

  filter "configurations:RelWithDebInfo"
    defines { "DEBUG" }
    runtime "Release"
    optimize "on"
    symbols "on"