import fileinput
import re
import sys
import inspect, os
from subprocess import call, Popen, PIPE
import shlex

# Minimum Python 3.3 required
assert sys.version_info >= (3,3)

version_file = False
arguments = sys.argv[1:]

# 1st argument: target directory
if len(arguments) > 0:
	target_directory = arguments[0]
	
# 2nd argument: target file to be updated
if len(arguments) > 1:
	version_file = arguments[1]
	

if target_directory == False:
	target_directory = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
if version_file == False:
	version_file = 'version.h'
	

# Extracting current version number
versionbuildno = 0
file = os.path.join(target_directory, version_file)
ofile = open(file, 'r')
ofilecontent = ofile.read()

m1 = []
r1 = re.compile(r"(#define VER_BUILDNO[\W]{1,})([\w]{1,})")
m1 = r1.findall(ofilecontent)

if len(m1) > 0:
	versionbuildno = int(m1[0][1])
	versionbuildno += 1
	
print('Current build no: ' + str(versionbuildno))

ofile.close()


# Updating version file
print('Updating version file ' + file + ' ...')

r2 = re.compile(r"^(#define VER_BUILDNO[\W]{1,})([\w]{1,})")
for line in fileinput.input(file, inplace=True):
	print(r2.sub(r"\g<1>" + str(versionbuildno), line), end="")