#!/usr/bin/python
#-*-coding:utf-8

import sys

num=int(sys.argv[1])

print 'num: %d'%num

two_num_list = bin(num)[2:]
gisu = len(bin(num))-3		# 지수

print '===================' 

result = 0

for two_num in two_num_list:
	if two_num == '1':
		print '3**%d = %d'%(gisu, 3 ** gisu)
		result = result + (3 ** gisu)
	gisu = gisu-1;

print result
