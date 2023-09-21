workspace "AndromedEngine"
  architecture "x86_64"    

  startproject "Andromeda"

  configurations
  {
    "Debug",
    "RelWithDebInfo",
    "Release",    
  }
  
  flags
  {
    "MultiProcessorCompile"
  }


group "Core"

project "Andromeda"
  kind "StaticLib"
  language "C++"
  cppdialect "C++2a"
  staticruntime "Off"

  -- Archivos del proyecto de visual
  location "build/%{prj.name}"

  -- Carpeta donde va a ir lo que compile
  targetdir "bin/%{cfg.buildcfg}"

  -- Carpeta donde van a ir los obj generados
  objdir "build/%{cfg.buildcfg}"

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  }

  -- Directorios donde estan los include
  includedirs
  {
    "include",
    "src",
  }
  
  -- Todos los ficheros que van a aparecer en la solucion
  files
  {
    "include/**.h",
    "src/**.cpp",
    "src/**.h",
    "premake5.lua"
  }

  -- Tipos de configuracion y sus flags
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

group ""

group "Examples"
  include "examples/Sandbox"
group ""
