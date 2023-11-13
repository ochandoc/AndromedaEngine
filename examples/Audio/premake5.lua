project "Audio"
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
    "_CRT_SECURE_NO_WARNINGS",
    "WITH_WINMM"
  }

  files
  {
    "src/**.cpp",
    "src/**.h",
    "../../deps/Soloud/soloud/include/**.h",
    "../../deps/Soloud/soloud/src/audiosource/**.cpp",        -- Archivos fuente de la carpeta audiosource
    "../../deps/Soloud/soloud/src/filter/**.cpp",             -- Archivos fuente de la carpeta filter
    "../../deps/Soloud/soloud/src/core/**.cpp",                 -- Archivos fuente de la carpeta core
    "../../deps/Soloud/soloud/src/backend/winmm/**.*",        -- Archivos fuente específicos para la plataforma Windows Multimedia (si es necesario)
    --"../../deps/soloud/src/**.cpp",
    "premake5.lua"
  }

  includedirs
  {
    "../../include",
    "../../deps/Soloud/soloud/include",
    "../../deps/Soloud/soloud/src/audiosource/**.cpp",        -- Archivos fuente de la carpeta audiosource
    "../../deps/Soloud/soloud/src/filter/**.cpp",             -- Archivos fuente de la carpeta filter
    "../../deps/Soloud/soloud/src/core/**.cpp",                 -- Archivos fuente de la carpeta core
    "../../deps/Soloud/soloud/src/backend/winmm/**.*",        -- Archivos fuente específicos para la plataforma Windows Multimedia (si es necesario)
    -- "../../deps/soloud/src",
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