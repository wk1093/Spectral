# use doxygen and moxygen

import subprocess
import os
import time
import shutil

subprocess.run("doxygen", shell=True)

sp = subprocess.run("python docgen/pylib/xml_to_json_parser/xml_to_json_parser.py docgen/xml docgen/output.json", shell=True)
sp.check_returncode()
sp = subprocess.run("python docgen/pylib/json_to_Markdown_converter/json_to_Markdown_converter.py docgen/output.json docs/", shell=True)
sp.check_returncode()

if os.path.isfile("./json_to_Markdown_converter.log"):
    os.remove("./json_to_Markdown_converter.log")
if os.path.isfile("./xml_to_json_parser.log"):
    os.remove("./xml_to_json_parser.log")

time.sleep(1)

# for all the generated markdown files, if there is any empty files (just a header and no content), remove them
for root, dirs, files in os.walk("docs"):
    for file in files:
        if file.endswith(".md"):
            file_path = os.path.join(root, file)
            with open(file_path, "r") as f:
                content = f.read()
                # if content.strip().startswith("# ") and "\n" not in content.strip():
                #     pass
                # else:
                #     continue
                if not content.strip().startswith("# ") or "\n" in content.strip():
                    continue
            os.remove(file_path)

# remove any empty directories in docs
for root, dirs, files in os.walk("docs"):
    for dir in dirs:
        dir_path = os.path.join(root, dir)
        # if not os.listdir(dir_path):
        #     os.rmdir(dir_path)
        # this doesn't work if a directory just contains empty directories
        # so we need to check if the directory is empty or not by actually recursing through it
        empty = True
        for dirpath, dirnames, filenames in os.walk(dir_path):
            if filenames:
                empty = False
                break
        if empty:
            shutil.rmtree(dir_path)
