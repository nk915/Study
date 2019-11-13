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
	def __init__(self): # 파이썬 생성자
		self.udp_socket = socket(AF_INET, SOCK_DGRAM);
		os.system('sar -v > sar_result.txt');
		ha_output = commands.getoutput('ls /flash/ | grep ha.conf -c');
		
		if ha_output > '1':
			os.system('cp /flash/ha.conf /flash/check_agent_mac.conf');
			os.system('cf /flash/check_agent_mac.conf de');

		self.log_file = open('/flash/log/alarm.log', "a");
	
	def __del__(self): # 파이썬 소명자
		os.system('rm -rf sar_result.txt');
		ha_output = commands.getoutput('ls /flash/ | grep ha.conf -c');
		if ha_output > '1':
			os.system('rm -rf /flash/check_agent_mac.conf');
	
	def getHwAddr(self): # MAC 주소를 뽑아 내는 함수.
		info = fcntl.ioctl(self.udp_socket.fileno(), 0x8927, struct.pack('256s', self.strAdapterName[:15]))
		return info[18:24];

	def getIpAddr(self): # IP 주소를 뽑아 내는 함수.
		info = fcntl.ioctl(self.udp_socket.fileno(), 0x8915, struct.pack('256s', self.strAdapterName[:15]))
		return info[20:24];
	
	def mactobinar(self, mac): # MAC 주소를 binary로 변화 해주는 함수.
		addr = ''
		temp = mac.replace(":", '');

		for i in range(0, len(temp), 2):
			addr = ''.join([addr, struct.pack('B', int(temp[i: i + 2], 16))]);

		return addr;

	def getvirtualHwAddr(self): # 이중화로 구성될 경우 이중화 MAC 주소 뽑아 내는 함수.
		f = open('/flash/check_agent_mac.conf');
		line = f.readline();
		line_split = line.split();
		return self.mactobinar(line_split[1]);
	
	def linuxhacheck(self): # 현재 이중화 시스템이 동작 check 함수
		output = commands.getoutput('ps -ef | grep heartbeat -c');
		if output > '1':
			return 1;
		return 0;
	
	def setting(self): # vpn.conf을 파싱하여 agent와 manager 정보 setting
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
				self.AgentMacAddr = self.getvirtualHwAddr(); # 이중화 서비스가 동작 할 경우 가상 MAC을 가지고 온다.
		else:
			self.AgentMacAddr = self.getHwAddr(); # 자기 자신의 MAC 주소를 뽑아 낸다.
		self.m_ulSenderIP = self.getIpAddr(); # 자기 자신의 IP 주소를 뽑아 낸다.
	
	def send_udp(self): # 알람을 전송하는 함수.
		packet_data = self.data();
		self.udp_socket.sendto(packet_data, (self.hostname, port))

	def data(self): # data를 조합하는 함수.
		data = ''
		data = data + self.AgentMacAddr  
		data = data + (self.strAdapterName + ("\x00"*(50-len(self.strAdapterName)))) 
		data = data + self.m_ulSenderIP 
		data = data + "\x00"* 4 # 알람과 관계된 IP
		data = data + "\x00"* 6 # 알람과 관계된 MAC  
		data = data + "\x00"* 15 # 알람과 관계된 컴퓨터 명
		data = data + "\x00"* 15 # 알람과 관계된 그룹명
		data = data + "\x00"* (4+6+6) # 정책에 설정괸 IP, MAC, MAC2
		data = data + struct.pack("<b", int(alarm_type));
		data = data + "\x00"* (1+7) # 시스템 접근 제어 & 예약 필드  

		return data

	'''
		sar check 함수.
		sar에 있는 file-sz를 check를 하여 threshold보다 넘을 경우 alarm을 보냄.
		이 함수는 threshold보다 넘을 경우 true, 안넘을 경우 false
	'''
	def count_file(self, list_split): # sar 파일에서 file-sz 위치를 뽑아 내는 함수.
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
				log_s = log_s + " : file sz 값(" + line_split[point_file_sz] + ")이 Threshod을 넘었습니다. \n";
				self.log_file.write(log_s);
				return 1;
		log_s = str(datetime.datetime.now());
		log_s = log_s + " : file sz [정상] \n";
		self.log_file.write(log_s);
		return 0;

p = alarm();
p.setting();
if p.sar_parsing() == 1: # sar 파싱으로 threshold보다 넘을 경우 알람을 보낸다.
	log_s = str(datetime.datetime.now());
	log_s = log_s + " : 알람 전송 PM(" + p.hostname + ")\n";
	p.log_file.write(log_s);
	p.send_udp();
