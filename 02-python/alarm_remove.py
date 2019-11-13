#!/usr/bin/python
# -*- coding:utf-8 -*-

import commands
import os

VERSION='1.0'
CRON_PATH='/var/spool/cron/root'

output = commands.getoutput('crontab -l')

print '== Cron alarm Remove Start =='

def shell_cmd(_cmd):
	print _cmd
	os.system(_cmd)


shell_cmd('cat /dev/null > %s'%(CRON_PATH))

for i in output.split('\n'):
	if 'alarm.py' in i and '#' not in i:
		shell_cmd('echo \"# %s\" >> %s'%(i, CRON_PATH))
	else:
		shell_cmd('echo \"%s\" >> %s'%(i, CRON_PATH))


print '== Cron alarm Remove Finish =='
