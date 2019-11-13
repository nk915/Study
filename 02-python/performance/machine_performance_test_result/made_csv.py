#!/usr/bin/python
#-*- coding:utf-8 -*-

import os
import csv
import time
import commands
import csv

def average(arg):
	sum = 0
	for data in arg:
		sum += data
	
	return sum/len(arg)


def snalist(fname, core):
	_sna = []
	which_file = './%s/%s_SNA.txt'%(fname, fname)
	fs = open(which_file, 'r')

	core_cnt = commands.getstatusoutput('grep -c processor /proc/cpuinfo')

	for line in fs.readlines():
		tmp = line.split()
		_sna.append(float(tmp[8])/core)

	fs.close()
	return _sna

def cpulist(fname):
	_cpu = []
	which_file = './%s/%s_CPU.txt'%(fname, fname)
	fs = open(which_file, 'r')
	
	for line in fs.readlines():
		tmp = float(line.split()[7])
#		tmp = float(line[34] + line[35] + line[36] + line[37] + line[38])
		_cpu.append(tmp)

	fs.close()
	return _cpu


def packetlist(fname):
	_packet = []
	which_file = './%s/%s_PER.txt'%(fname, fname)
	fs = open(which_file, 'r')
	
	for line in fs.readlines():
		tmp = line.split()
		if tmp[4] == '[A/vlan110]':
			_packet.append(int(tmp[21]))
	
	fs.close()
	return _packet


def made_csv(_testname, _today, _cpu):
	csv_file = '%s.csv'%(_testname)
	print _packet

	with open(csv_file, 'wt') as csvfile:
		wt = csv.writer(csvfile)
		wt.writerow(('', '', _testname))
		wt.writerow(('', 'test_date:', _today))
		wt.writerow(('', 'MIN', 'MAX', 'AVERAGE' ))
		wt.writerow(('CPU', min(_cpu), max(_cpu), average(_cpu)))


def main():
	today = time.localtime()
	cpu = []
	sna = []
	packet = []
	
	tmp_dir = raw_input('dir_name:')
	core = input('core:')

	date = '%d/%d/%d'%(today.tm_year, today.tm_mon, today.tm_mday)		
	print date

	
	for i in range(0, 5):
		dir_name = '%s%d'%(tmp_dir,i) 
		print '------------- %s --------------------'%(dir_name)
		cpu = cpulist(dir_name)
		print 'CPU -- MIN: %.2f,  MAX: %.2f,  AV: %.2f'%(100 - max(cpu), 100 - min(cpu), 100 - average(cpu))

		sna = snalist(dir_name, core)
		print 'SNA -- MIN: %.2f,  MAX: %.2f,  AV: %.2f'%(min(sna), max(sna), average(sna))
		print '---------------------------------------------------'
		print ''
#	sna = snalist(testname, core)
#	packet = packetlist(testname)
	
#	tmp = made_csv(testname, date, cpu, sna, packet)
	print '__End__'


if __name__ == "__main__":
	main()

