@echo off

for /d %%d in (..\examples\*) do (
  copy /y .\utils\copy_data.bat %%d\copy_data.bat
)

cd %~dp0..

premake5.exe vs2022

pause