import sys
import re

if len(sys.argv) != 4:
    print('Usage: python3 create.py <type> <name> <desc>')
    exit(1)

type = sys.argv[1]
type_pattern = re.compile("^(c|py|sh)$")
if not type_pattern.match(type):
    print('Type must be c, py or sh')
    exit(1)

name = sys.argv[2]
name_pattern = re.compile("^[a-zA-Z0-9_]+$")
if not name_pattern.match(name):
    print('Name must be alphanumeric')
    exit(1)

desc = sys.argv[3]

src_path = 'src/' if type== 'c' else 'scripts/' 

imports = '#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <unistd.h>\n#include <sys/wait.h>\n#include <time.h>\n#include "ex.h"'

new_file = open(src_path + name + '.' + type, 'w')

if type == 'c':
    new_file.write(imports + '\n\n')
    new_file.write('int ' + name + '(int argc, char **argv) {\n\n\treturn EXIT_SUCCESS;\n}')

    new_file.close()

    ex = open(src_path + 'ex.h', 'a')
    ex.write('\n/**\n * ' + desc + '\n */\nextern int ' + name + '(int argc, char **argv);\n\n')
    ex.close()

if type == "py":
    new_file.write('import sys\nimport subprocess\nimport time\n\n')
    new_file.write('def ' + name + '(args):\n\treturn 0\n\n')

    new_file.write('if __name__ == "__main__":\n\t' + name + '(sys.argv[1:])\n')

    new_file.close()


