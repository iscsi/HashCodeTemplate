import sys
import os
import subprocess
from shutil import copyfile


if not os.path.exists("build"):
    os.makedirs("build")

filename = "A"
if len(sys.argv) > 1:
	filename = sys.argv[1]

#cmakelists start
with open("CMakeLists_template.txt", 'r') as file :
  filedata = file.read()

filedata = filedata.replace('template', filename)

with open('CMakeLists.txt', 'w') as file:
  file.write(filedata)

#cmakelists end
#compile.bat  
with open("compile_template.cmd", 'r') as file :
  filedata = file.read()

filedata = filedata.replace('template', filename)

with open('compile.cmd', 'w') as file:
  file.write(filedata)

#compile.bat  end

#create in/ out directories

os.mkdir("in")
os.mkdir("out")

#remove files: template.cpp, template.h compile_template.cmd CMakeLists_template.py

os.remove("compile_template.cmd")
os.remove("CMakeLists_template.txt")


os.chdir("build")
subprocess.call('cmake -G "Visual Studio 15 2017 Win64" ..\\')