#!/usr/bin/python
# -*- coding:utf-8 -*-

import commands
import os


class test:
	t1 = 1
	t2 = 2


	def __init__(self, _t1=1, _t2=2):
		self.t1 = _t1
		self.t2 = _t2

	def _print(self):
		print 't1 = %d , t2 = %d'%(self.t1, self.t2) 



tmp_class_list = list()
tmp_class_list.append(test())
tmp_class_list.append(test(1,3))

print 'len : %d'%len(tmp_class_list)

for tmp_class in tmp_class_list:
	tmp_class._print()








