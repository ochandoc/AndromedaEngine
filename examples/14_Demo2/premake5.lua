project "14_Demo2"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "Off"

  location "build"

  targetdir "bin/%{cfg.buildcfg}"
  objdir "build/obj/%{cfg.buildcfg}"

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

  postbuildcommands 
  {
    [[
      cd %{wks.location}examples\%{prj.name}\
      copy_data.bat
    ]]
  }

  conan_config = get_conan_config()
  setup_dependencies_exe(conan_config)

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