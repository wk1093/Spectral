# use doxygen and moxygen

import subprocess
import os
# from docgen.pylib.json_to_Markdown_converter.json_to_Markdown_converter import main as j2md
# from docgen.pylib.xml_to_json_parser.xml_to_json_parser import main as x2j
import sys

subprocess.run("doxygen", shell=True)

subprocess.run("python docgen/pylib/xml_to_json_parser/xml_to_json_parser.py docgen/xml docgen/output.json", shell=True)
subprocess.run("python docgen/pylib/json_to_Markdown_converter/json_to_Markdown_converter.py docgen/output.json docs/", shell=True)

if os.path.isfile("./json_to_Markdown_converter.log"):
    os.remove("./json_to_Markdown_converter.log")
if os.path.isfile("./xml_to_json_parser.log"):
    os.remove("./xml_to_json_parser.log")