import sys
import subprocess
import time
from threading import Thread

"""
Scrivere un programma python o uno script bash che consenta di lanciare parallelamente comandi
para.py sleep 20 // ls -l // ps // sleep 3
para.sh sleep 20 // ls -l // ps // sleep 3
deve eseguire concorrentemente i vari comandi separati da // e aspettare la terminazione di tutti
"""
def para(args):

	cmds = args[0].split("//")

	threads = []

	for cmd in cmds:
		cmd = cmd.strip()
		if cmd == "":
			continue
		print(cmd)
		thread = Thread(target=subprocess.run, args=(cmd,), kwargs={'shell':True})
		thread.start()

		threads.append(thread)

	for thread in threads:
		thread.join()

	print("We're done!")

	return 0

if __name__ == "__main__":
	para(sys.argv[1:])
