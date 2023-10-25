import os
import subprocess
import platform
import importlib.util
import subprocess

def CheckPythonPackages(name):
  if importlib.util.find_spec(name) is None:
    permission_garanted = False
    while not permission_garanted:
      reply = str(input("Would you like to install Python package '{0:s}'? [Y/N]: ".format(name))).lower().strip()[:1]
      if reply == 'n':
        return False
      permission_garanted = (reply == 'y')

    print(f"Installing {name} module...")
    subprocess.call(['python3', '-m', 'pip', 'install', name])
    return CheckPythonPackages(name)
  return True

packages_needed = ['requests']

for name in packages_needed:
  if not CheckPythonPackages(name):
    print("Can't install the packages needed for the setup.")
    exit()
  
from setup_premake import Premake 

installed = Premake.Validate()

if installed:
  if platform.system() == "Windows":
    reply = str(input("Would you like to build dependencies? [Y/N]: ")).lower().strip()[:1]
    if reply == 'y':
      print("Building dependencies")
      subprocess.call([os.path.abspath("build_dependencies.bat"), "nopause"])
    print("\nRunning premake...")
    subprocess.call([os.path.abspath("premake.bat"), "nopause"])
  
  print("\nSetup completed!")
else:
  print("Requires Premake to generate project files.")