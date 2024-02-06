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

function setup_dependencies_lib(conan_config)
  for i,build_cfg in ipairs(build_configurations) do
    local cfg = conan_config[build_cfg]
    filter("configurations:"..build_cfg)
      includedirs{ cfg["includedirs"] }
      libdirs{ cfg["libdirs"] }
      links{ cfg["libs"] }
      links{ cfg["frameworks"] }
      defines{ cfg["defines"] }
      bindirs{ cfg["bindirs"] }
  end
end

function setup_dependencies_exe(conan_config)
  for i,build_cfg in ipairs(build_configurations) do
    local cfg = conan_config[build_cfg]
    filter("configurations:"..build_cfg)
      links{ cfg["system_libs"] }
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
    "MultiProcessorCompile",
    "FatalWarnings",
  }

group "Core"

project "Andromeda"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "Off"

  -- Archivos del proyecto de visual
  location "build/%{prj.name}"

  -- Carpeta donde va a ir lo que compile
  targetdir "bin/%{cfg.buildcfg}"

  -- Carpeta donde van a ir los obj generados
  objdir "build/%{prj.name}/%{cfg.buildcfg}"

  defines
  {
    "_CRT_SECURE_NO_WARNINGS",
    "AND_BUILD_LIB",
  }

  -- Directorios donde estan los include
  includedirs
  {
    "include",
    "src",
  }
  
  pchheader "andpch.hpp"
  pchsource "src/andpch.cpp"
  forceincludes { "andpch.hpp" }

  -- Todos los ficheros que van a aparecer en la solucion
  files
  {
    "include/**.h",
    "include/**.hpp",
    "src/Andromeda/**.*",
    "src/Graphics/**.*",
    "include/andpch.hpp",
    "src/andpch.cpp",
    "premake5.lua"
  }
  
  conan_cfg = get_conan_config()
  setup_dependencies_lib(conan_cfg)

  filter "system:windows"
    files 
    {
        "src/Platform/Windows/**.*",
    }

  filter { "options:GraphicsAPI=OpenGL" }
    defines { "AND_OPENGL", }
    files
    {
      "src/Backends/OpenGL/**.*",
    }

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
  include "examples/01_Window"
  include "examples/02_Logger"
  include "examples/03_Memory"
  include "examples/04_HelloTriangle"
  include "examples/05_EntityComponentSystem"
  include "examples/06_Triangles"
  include "examples/07_ResourceSystem"
  include "examples/08_OBJs"
  include "examples/09_OBJs_Lights"
group ""

newoption
{
  trigger = "GraphicsAPI",
  value = "API",
  description = "Choose a particular API for rendering",
  allowed = { {"OpenGL",}, {"Vulkan",}, },
  default = "OpenGL",
}
