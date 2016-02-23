#!/usr/bin/python
import os, sys
from datetime import datetime

time = datetime.now().strftime("%B %d %H:%M hours")
status = "git status"
add = 'git add .'
commit = "git commit -a -m " + "\"" +  time + "\""
pull = "git pull -u vepc master"
os.system(status)
proceed = raw_input('Proceed? (y/a/n): ')
if (proceed == 'y'):
	os.system(pull)
elif (proceed == 'a'):
	os.system(add)
	os.system(commit)
	os.system(pull)
else:
	print "Aborting pull.."	
