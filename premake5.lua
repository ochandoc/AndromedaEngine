build_configurations = { "Debug", "RelWithDebInfo", "Release" }

function get_conan_config()
  local configurations = {}
  for i,build_cfg in ipairs(build_configurations) do
    include("build/deps/"..build_cfg.."/conanbuildinfo.premake.lua")
    configurations[build_cfg] = {}
    local cfg = configurations[build_cfg]
    cfg["includedirs"] = conan_includedirs
    cfg["libdirs"] = conan_libdirs
    cfg["bindirs"] = conan_bindirs
    cfg["libs"] = conan_libs
    cfg["system_libs"] = conan_system_libs
    cfg["defines"] = conan_defines
    cfg["frameworks"] = conan_frameworks
  end
  return configurations
end

function setup_dependencies(conan_config)
  for i,build_cfg in ipairs(build_configurations) do
    local cfg = conan_config[build_cfg]
    filter("configurations:"..build_cfg)
      includedirs{ cfg["includedirs"] }
      libdirs{ cfg["libdirs"] }
      links{ cfg["libs"] }
      links{ cfg["system_libs"] }
      links{ cfg["frameworks"] }
      defines{ cfg["defines"] }
      bindirs{ cfg["bindirs"] }
  end
end

workspace "AndromedEngine"
  architecture "x86_64"    

  startproject "Sanbox"

  configurations
  {
    build_configurations    
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
  objdir "build/%{prj.name}/%{cfg.buildcfg}"

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
    "include/**.hpp",
    "src/**.cpp",
    "src/**.h",
    "src/**.hpp",
    "premake5.lua"
  }

  pchheader "andpch.hpp"
  pchsource "src/andpch.cpp"
  forceincludes { "andpch.hpp" }

  conan_cfg = get_conan_config()
  setup_dependencies(conan_cfg)

  -- Tipos de configuracion y sus flags
  filter "configurations:Debug"
    defines { "DEBUG" }
    runtime "Debug"
    optimize "Debug"
    symbols "on"
  
  filter "configurations:Release"
    defines { "NDEBUG" }
    runtime "Release"
    optimize "Speed"
    symbols "off"

  
  filter "configurations:RelWithDebInfo"
    defines { "DEBUG" }
    runtime "Release"
    optimize "on"
    symbols "on"

group ""

group "Examples"
  include "examples/Sandbox"
group ""


