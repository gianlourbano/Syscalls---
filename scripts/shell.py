import sys
import subprocess
import time
import os

def shell():
	cwd = os.getcwd()

	while(1):
		print(f"[{cwd}] >>> ", end = "")
		try:
			read = input()
		except EOFError:
			print("\n")
			return 0
		if read == "logout":
			return 0

		read = read.split(" ")

		if read[0] == "cd":
			os.chdir(read[1])
			if read[1] == "..":
				cwd = cwd[:cwd.rfind("/")]
			elif read[1] == ".":
				pass
			else:
				cwd = cwd + "/" + read[1]
		else:
			res = subprocess.run(read, shell=False, check=True)
		
	
	return 0

if __name__ == "__main__":
	shell()
