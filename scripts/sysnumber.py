import sys
import subprocess
import time

	# Esegui il comando per ottenere l'output desiderato
output = subprocess.check_output('echo "#include<sys/syscall.h>" | gcc -dD -E -', shell=True, text=True)
# Crea un dizionario per memorizzare l'associazione syscall-numero
syscall_mapping = {}
# Analizza l'output del comando gcc per ottenere le associazioni
for line in output.splitlines():
	if line.startswith('#define __NR_'):
		parts = line.split()
		syscall_name = parts[1][5:]  # Rimuovi il prefisso "__NR_"
		syscall_num = int(parts[2])
		syscall_mapping[syscall_name] = syscall_num

# Funzione per ottenere il numero di syscall dato il nome
def get_syscall_number(syscall_name):
    print( syscall_mapping.get(syscall_name, "Syscall non trovata"))

# Funzione per ottenere il nome della syscall dato il numero
def get_syscall_name(syscall_num):
	for name, num in syscall_mapping.items():
		if num == syscall_num:
			print(name)
			return
	print("Numero syscall non trovato")

def sysnumber(args):
	if len(args) < 1:
		print("Usage: sysnumber.py <syscall_name/number>")
		return 1

	syscall = args[0]

	if syscall.isdigit():
		get_syscall_name(int(syscall))
	else:
		get_syscall_number(syscall)

	return 0

if __name__ == "__main__":
	sysnumber(sys.argv[1:])
