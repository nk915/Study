#!/usr/bin/python
# -*- coding:utf-8 -*-

'''
	Moudlue Name   : alarm.py
	Last Update    : 2012.11.21

	Copyright (c) 2012 NetMan. All rights reserved.
	E-mail	       : kals25@netman.co.kr
'''

import sys, os, re, struct
import time, fcntl, commands
import datetime
from socket import *

# ----------------------------
port = 7884
alarm_type = 71
threshold = 20000
# ----------------------------

class alarm: 
	def __init__(self): # ���̽� ������
		self.udp_socket = socket(AF_INET, SOCK_DGRAM);
		os.system('sar -v > sar_result.txt');
		ha_output = commands.getoutput('ls /flash/ | grep ha.conf -c');
		
		if ha_output > '1':
			os.system('cp /flash/ha.conf /flash/check_agent_mac.conf');
			os.system('cf /flash/check_agent_mac.conf de');

		self.log_file = open('/flash/log/alarm.log', "a");
	
	def __del__(self): # ���̽� �Ҹ���
		os.system('rm -rf sar_result.txt');
		ha_output = commands.getoutput('ls /flash/ | grep ha.conf -c');
		if ha_output > '1':
			os.system('rm -rf /flash/check_agent_mac.conf');
	
	def getHwAddr(self): # MAC �ּҸ� �̾� ���� �Լ�.
		info = fcntl.ioctl(self.udp_socket.fileno(), 0x8927, struct.pack('256s', self.strAdapterName[:15]))
		return info[18:24];

	def getIpAddr(self): # IP �ּҸ� �̾� ���� �Լ�.
		info = fcntl.ioctl(self.udp_socket.fileno(), 0x8915, struct.pack('256s', self.strAdapterName[:15]))
		return info[20:24];
	
	def mactobinar(self, mac): # MAC �ּҸ� binary�� ��ȭ ���ִ� �Լ�.
		addr = ''
		temp = mac.replace(":", '');

		for i in range(0, len(temp), 2):
			addr = ''.join([addr, struct.pack('B', int(temp[i: i + 2], 16))]);

		return addr;

	def getvirtualHwAddr(self): # ����ȭ�� ������ ��� ����ȭ MAC �ּ� �̾� ���� �Լ�.
		f = open('/flash/check_agent_mac.conf');
		line = f.readline();
		line_split = line.split();
		return self.mactobinar(line_split[1]);
	
	def linuxhacheck(self): # ���� ����ȭ �ý����� ���� check �Լ�
		output = commands.getoutput('ps -ef | grep heartbeat -c');
		if output > '1':
			return 1;
		return 0;
	
	def setting(self): # vpn.conf�� �Ľ��Ͽ� agent�� manager ���� setting
		f = open('/flash/vpa.conf');
		line_count = 1;
		line = f.readline();
		line_split = line.split();
		self.hostname = line_split[1];
		while line:
			line = f.readline();
			line_count = line_count + 1;
			if line_count == 4:
				line_split = line.split();
				self.strAdapterName = line_split[0];
				break;
		ha_output = commands.getoutput('ls /flash/ | grep ha.conf -c');
		if ha_output > '1':
			if self.linuxhacheck() == 1:
				self.AgentMacAddr = self.getvirtualHwAddr(); # ����ȭ ���񽺰� ���� �� ��� ���� MAC�� ������ �´�.
		else:
			self.AgentMacAddr = self.getHwAddr(); # �ڱ� �ڽ��� MAC �ּҸ� �̾� ����.
		self.m_ulSenderIP = self.getIpAddr(); # �ڱ� �ڽ��� IP �ּҸ� �̾� ����.
	
	def send_udp(self): # �˶��� �����ϴ� �Լ�.
		packet_data = self.data();
		self.udp_socket.sendto(packet_data, (self.hostname, port))

	def data(self): # data�� �����ϴ� �Լ�.
		data = ''
		data = data + self.AgentMacAddr  
		data = data + (self.strAdapterName + ("\x00"*(50-len(self.strAdapterName)))) 
		data = data + self.m_ulSenderIP 
		data = data + "\x00"* 4 # �˶��� ����� IP
		data = data + "\x00"* 6 # �˶��� ����� MAC  
		data = data + "\x00"* 15 # �˶��� ����� ��ǻ�� ��
		data = data + "\x00"* 15 # �˶��� ����� �׷��
		data = data + "\x00"* (4+6+6) # ��å�� ������ IP, MAC, MAC2
		data = data + struct.pack("<b", int(alarm_type));
		data = data + "\x00"* (1+7) # �ý��� ���� ���� & ���� �ʵ�  

		return data

	'''
		sar check �Լ�.
		sar�� �ִ� file-sz�� check�� �Ͽ� threshold���� ���� ��� alarm�� ����.
		�� �Լ��� threshold���� ���� ��� true, �ȳ��� ��� false
	'''
	def count_file(self, list_split): # sar ���Ͽ��� file-sz ��ġ�� �̾� ���� �Լ�.
		point_file_sz = 0;

		for split in list_split:
			if split == "file-sz":
				return int(point_file_sz);

			point_file_sz = point_file_sz + 1;
		
		return 0;

	def sar_parsing(self):
		f = open('sar_result.txt');
		line = f.readline();
		point_file_sz = 0;

		while line:
			line = f.readline();
			line_split = line.split();
			list_count = len(line_split);
		
			if list_count == 0:
				continue;
			
			if self.count_file(line_split) != 0:
				point_file_sz = self.count_file(line_split);
				continue;
			
			if len(line_split) < 8:
				continue;

			if threshold < int(line_split[point_file_sz]):
				log_s = str(datetime.datetime.now());
				log_s = log_s + " : file sz ��(" + line_split[point_file_sz] + ")�� Threshod�� �Ѿ����ϴ�. \n";
				self.log_file.write(log_s);
				return 1;
		log_s = str(datetime.datetime.now());
		log_s = log_s + " : file sz [����] \n";
		self.log_file.write(log_s);
		return 0;

p = alarm();
p.setting();
if p.sar_parsing() == 1: # sar �Ľ����� threshold���� ���� ��� �˶��� ������.
	log_s = str(datetime.datetime.now());
	log_s = log_s + " : �˶� ���� PM(" + p.hostname + ")\n";
	p.log_file.write(log_s);
	p.send_udp();
