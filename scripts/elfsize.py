import sys
import subprocess
import time
import os
import stat

def is_elf(file):
    with open(file, "rb") as f:
        magic = f.read()[:4]
        if magic == b'\x7fELF':
            return True
    return False

def elfsize(args):
	cwd = os.getcwd()

	tot_count = 0

	for i in range(0, len(args)):
		for root, dir, files in os.walk(args[i]):
			for file in files:
				filepath = os.path.join(root, file)
				if is_elf(filepath):
					tot_count += os.stat(filepath).st_size
			break
	
	if len(args) == 0:
		for root, dir, files in os.walk("./"):
			for file in files:
				filepath = os.path.join(root, file)
				if is_elf(filepath):
					tot_count += os.stat(filepath).st_size
			break

	print("Total size of ELF files: " + str(tot_count) + " bytes")
	return 0

if __name__ == "__main__":
	elfsize(sys.argv[1:])
