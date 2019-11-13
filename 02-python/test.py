#!/usr/bin/python

if(1 or 0 or (0 and 0)):
	print '1'
if((1 or 0 or 0) and 0):
	print '2'
if(1 or 0 or 0 and 0):
	print '3'
if((1 or 0) or (0 and 0)):
	print '4'
if(1 or (0 or (0 and 0))):
	print '5'





print '================================'

if(0 or 1 or (0 and 0)):
	print '1'
if((0 or 1 or 0) and 0):
	print '2'
if(0 or 1 or 0 and 0):
	print '3'
if((0 or 1) or (0 and 0)):
	print '4'
if(0 or (1 or (0 and 0))):
	print '5'
