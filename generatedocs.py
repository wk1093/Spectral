# use doxygen and moxygen

import subprocess
import os
import time
import shutil

subprocess.run("doxygen", shell=True)

sp = subprocess.run("moxygen docgen/xml/ -a -l cpp -g -o docs/%s.md", shell=True)