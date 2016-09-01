import fileinput
import re
import sys
import inspect, os
from subprocess import call, Popen, PIPE
import shlex

target_directory = False
version_file = False
arguments = sys.argv[1:]
if len(arguments) > 0:
	target_directory = arguments[0]
if len(arguments) > 1:
	version_file = arguments[1]
	
if target_directory == False:
	target_directory = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
if version_file == False:
	version_file = 'version.h'
	
print('Target directory: ' + target_directory)

cmd = 'git -C "' + target_directory + '" rev-parse --short HEAD'
print(cmd)

p = Popen(shlex.split(cmd), stdin=PIPE, stdout=PIPE, stderr=PIPE, shell=True)
output, err = p.communicate()
rc = p.returncode

gitcurrev = str()
if (rc == 0): 
	gitcurrev = output.decode('UTF-8').rstrip('\n')
else:
	print('Error during Popen: ' + str(rc) + ', Error: ' + str(err))
	
print('Git current revision hash (short): ' + gitcurrev)


file = os.path.join(target_directory, version_file)
print('Updating version file ' + file + ' ...')


r1 = re.compile(r"^(#define VER_BUILD[\W]{1,})([\w]{1,})")
for line in fileinput.input(file, inplace=True):
	print(r1.sub(r"\g<1>" + gitcurrev, line), end="")