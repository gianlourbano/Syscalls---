import sys
import subprocess
import time
import os

def tree_printer(root):
	res = []
	for root, dirs, files in os.walk(root):
		for d in dirs:
			res.append(os.path.join(root, d))
		for f in files:
			res.append (os.path.join(root, f))
	return res

def get_ext(filename):
	return os.path.splitext(filename)[-1].lower()

def indent_c(args):
	wdir = "./"
	if len(args) > 0:
		wdir = args[0]

	files = tree_printer(wdir)
	print(files)

	for f in files:
		if get_ext(f) == '.c' or get_ext(f) == '.h':
			print(f"Indenting {f} ...")
			subprocess.check_output(f"ex -n '+norm!gg=G' +wq {f}")

	return 0

if __name__ == "__main__":
	indent_c(sys.argv[1:])
