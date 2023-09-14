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

def count_lines(filename):
	lines = 0
	with open(filename, 'r') as f:
		for line in f:
			lines += 1
	return lines

def countsrc(args):
	if len(args) < 1:
		print("Usage: countsrc.py <path>")
		return 1

	path = args[0]

	print("Counting lines of code in " + path + "...")

	files = tree_printer(path)

	srcs = [[], [], []]

	for f in files:
		if get_ext(f) == '.c':
			srcs[0].append(f)
		elif get_ext(f) == '.h':
			srcs[1].append(f)
		elif f.endswith('Makefile') or f.endswith('makefile'):
			srcs[2].append(f)

	counts = [0, 0, 0]
	exts = ['.c', '.h', 'Makefile']

	for ext in srcs:
		for f in ext:
			c = count_lines(f)
			print(f"{f[2:]} {c}")
			counts[srcs.index(ext)] += c
		print(f"tot {exts[srcs.index(ext)]}: {counts[srcs.index(ext)]}\n")
	print(f"tot source: {sum(counts)}")

	return 0

if __name__ == "__main__":
	countsrc(sys.argv[1:])
