import sys
import os
from pathlib import Path

import requests
import os

from zipfile import ZipFile	

def DownloadFile(url, filepath):
  response = requests.get(url)
  with open(filepath, 'wb') as file:
    file.write(response.content)

def UnzipFile(filepath, deletezip=True):
  with ZipFile(filepath, 'r') as file:
    file.extract('premake5.exe', '../')

  if deletezip:
    os.remove(filepath)


class Premake:
  premake_version = "5.0.0-beta2"
  premake_zip_urls = f"https://github.com/premake/premake-core/releases/download/v{premake_version}/premake-{premake_version}-windows.zip"
  premake_directory = "../"

  @classmethod
  def Validate(self):
    if (not self.PremakeInstalled()):
      print("Premake is not installed.")
      return False
    print("Premake detected")
    return True

  @classmethod
  def PremakeInstalled(self):
    exe_path = Path(f"{self.premake_directory}/premake5.exe")
    if (not exe_path.exists()):
      return self.InstallPremake()
    return True
  
  @classmethod
  def InstallPremake(self):
    premakePath = f"{self.premake_directory}/premake-{self.premake_version}-windows.zip"
    permission_garanted = False
    while not permission_garanted:
      reply = str(input("Would you like to download premake.exe [Y/N]: ")).lower().strip()[:1]
      if reply == 'n':
        return False
      permission_garanted = (reply == 'y')
    DownloadFile(self.premake_zip_urls, premakePath)
    UnzipFile(premakePath)
    return True