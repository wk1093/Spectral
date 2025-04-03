# Requires doxygen and moxygen to be installed

import subprocess

try:
    subprocess.run("doxygen --version", shell=True, check=True)
except subprocess.CalledProcessError:
    print("Doxygen is not installed. Please install it first.")
    exit(1)

try:
    subprocess.run("moxygen --version", shell=True, check=True)
except subprocess.CalledProcessError:
    print("Moxygen is not installed. Please install it first.")
    exit(1)

subprocess.run("doxygen", shell=True)
sp = subprocess.run("moxygen docgen/xml/ -H -l cpp --html-anchors -o docs/api.md", shell=True)