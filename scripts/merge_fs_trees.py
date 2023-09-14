import sys
import subprocess
import time
import shutil
import os

def merge_trees(src1, src2, dest):
    if not os.path.exists(dest):
        os.makedirs(dest)

    # Copia i contenuti dell'albero 1 nella destinazione
    for root, _, files in os.walk(src1):
        for file in files:
            src_path = os.path.join(root, file)
            dest_path = os.path.join(dest, os.path.relpath(src_path, src1))
            merge_or_copy_file(src_path, dest_path)

    # Copia i contenuti dell'albero 2 nella destinazione
    for root, _, files in os.walk(src2):
        for file in files:
            src_path = os.path.join(root, file)
            dest_path = os.path.join(dest, os.path.relpath(src_path, src2))
            merge_or_copy_file(src_path, dest_path)

def merge_or_copy_file(src_path, dest_path):
    if os.path.exists(dest_path):
        # Se il file esiste già, concatena i contenuti
        with open(src_path, 'rb') as src_file, open(dest_path, 'ab') as dest_file:
            dest_file.write(src_file.read())
    else:
        # Altrimenti, copia semplicemente il file
        shutil.copy2(src_path, dest_path)

def merge_fs_trees(args):
	print(args)
	if len(args) != 3:
		print("Usage: merge_fs_trees.py <src1> <src2> <dest>")
		return 1

	src1 = args[0]
	src2 = args[1]
	dest = args[2]

	merge_trees(src1, src2, dest)

	return 0

if __name__ == "__main__":
	merge_fs_trees(sys.argv[1:])
