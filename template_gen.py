import sys
import os
import subprocess
from shutil import copyfile


if not os.path.exists("build"):
    os.makedirs("build")

filename = "A"
if len(sys.argv) > 1:
	filename = sys.argv[1]

#copy template.cpp 

copyfile("template.cpp", filename + ".cpp")

#copy template.h utils.h

with open("template.h", 'r') as file :
  filedata = file.read()

filedata = filedata.replace('template.cpp', filename + '.cpp')

with open('utils.h', 'w') as file:
  file.write(filedata)

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

#remove files: template.cpp, template.h compile_template.cmd CMakeLists_template.py

os.remove("template.cpp")
os.remove("template.h")
os.remove("compile_template.cmd")
os.remove("CMakeLists_template.txt")


os.chdir("build")
subprocess.call('cmake -G "Visual Studio 15 2017 Win64" ..\\')