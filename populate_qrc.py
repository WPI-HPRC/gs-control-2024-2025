import os
import pathlib

rootPath = pathlib.Path = pathlib.Path(__file__).parent.resolve()
dirPath = rootPath / "Resources"
filePath = dirPath / "Resources.qrc"

prefix = "/"

file = open(filePath, "w")

file.write("<RCC>\n")
file.write(f"\t<qresource prefix=\"{prefix}\">\n")

for path, subdirs, files in os.walk(dirPath):
    for name in files:
        if name.startswith(".") or name == "Resources.qrc":
            continue

        file.write(f"\t\t<file>{os.path.join(path, name).split('Resources/')[1]}</file>\n")

file.write("\t</qresource>\n")
file.write("</RCC>\n")