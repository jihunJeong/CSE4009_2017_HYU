# !/usr/bin/env python

import os
import sys
import subprocess
from subprocess import Popen, PIPE

def is_number(s):
	try:
		int(s)
		return True
	except ValueError:
		return False

var1 = sys.argv[1]


p = Popen("git ls-files `git rev-parse --show-toplevel`", stdin = PIPE, stdout = PIPE, shell = True)
print "-------------------------------------------------------"
line_list = [None] * 10
subline_list = [None] * 10
cnt = 0
num = 0
while cnt < 40000:
	result = p.stdout.readline().decode('utf-8').strip()

	if result.lower().find(var1.lower()) != -1:
		num = 1 + num
		line_list[num -1] = result
		line = result + " (%d)" % num
		print line
	if num == 10:
		break

	cnt = cnt + 1

print("Enter file shortcut (shown on the right) or keyword to further regine the search:")
input1 = raw_input()

if is_number(input1):
	input1 = int(input1)
	if input1 > 0 and input1 < num + 1:
		os.system("vim %s" % line_list[input1 - 1])
	else:
		print "Out of range"

else:
	cnt = 0
	for i in range(0, num):
		if line_list[i].find(input1) != -1:
			subline_list[cnt] = line_list[i]
			cnt = cnt + 1

	if cnt == 1:
		os.system("vim %s" % line_list[0])
	else :
		cnt = 0
		print "-------------------------------------------------------"
		for i in range(0, num):
			if line_list[i].find(input1) != -1:
				subline_list[cnt] = line_list[i]
				cnt = cnt + 1
				print line_list[i] + " (%d)" % cnt

		print("Enter file shortcut (shown on the right) or keyword to further regine the search:")
		input1 = raw_input()

		if is_number(input1):
			input1 = int(input1)
			if input1 > 0 and input1 < cnt + 1:
				os.system("vim %s" % line_list[input1 - 1])
			else:
				print "Out of range"

		else:
			cnt = 0
			for i in range(0, num):
				if line_list[i].find(input1) != -1:
					subline_list[cnt] = line_list[i]
					cnt = cnt + 1
					os.system("vim %s" % line_list[i])