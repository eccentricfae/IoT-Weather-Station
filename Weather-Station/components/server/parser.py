import os
import pathlib
from pathlib import Path

# Remember the current working directory
current_dir = pathlib.Path().resolve()
# Get the path to the direcotry of the this script
dir = pathlib.Path(__file__).parent.resolve()
dir = str(dir)

# Go into the pages directory with the html pages / files
os.chdir(dir + '/pages')

break_token = '// !@#$%'

# Get a list off all and only .html files
files_tmp = [file for file in os.listdir() if os.path.isfile(file)]
files = [file for file in files_tmp if file.endswith('.html')]

for file in files:
    name = Path(file).stem

    # Create temporary output file
    output = open('tmp_' + name + '.c', 'w')

    output.write('#include "server.h"\n\n')
    # Create the char array w/ the .html file's data
    output.write('char const ' + name + '_data[] = \n')
    source = open(file, 'r')
    # Add the file's data into the char array
    for line in source:
        line = line.replace('"', '\'')
        line = line.rstrip()
        output.write('\t"' + line + '\\' + 'n' + '"\n')
    # "End" the array initialization
    output.write(';\n\n')
    source.close()

    break_token_found = False

    # Copy the original source file (<name>.c), excluding everything before the break token (i.e. HTML data parsed into C char array)
    source = open(name + '.c', 'r')
    if source.closed:
            print("Failed to open the " + name + '.c' + "file!")
            exit(-1)
    for line in source:
        if break_token_found == False:
            if line.startswith(break_token):
                break_token_found = True
            else:
                continue
        output.write(line)

    source.close()
    output.close()
    # Remove the old file and replace it with the new one
    os.remove(name + '.c')
    os.rename('tmp_' + name + '.c', name + '.c')

os.chdir(current_dir)