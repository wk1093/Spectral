# use doxygen and moxygen

import subprocess
import os
import time
import shutil

subprocess.run("doxygen", shell=True)

sp = subprocess.run("moxygen docgen/xml/ -H -l cpp -o docs/api.md", shell=True)