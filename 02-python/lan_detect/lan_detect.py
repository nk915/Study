#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys
import os

ETH='eth1'
PROBE_IP='192.168.74.10'
PROBE_MAC='00:30:18:a7:95:99'
BROADCAST_MAC='ff:ff:ff:ff:ff:ff'
ZERO_MAC='00:00:00:00:00:00'
LAN_RANGE=['192.168.74.', '192.168.174.','203.255.174.','192.168.104.']


def shell_cmd(_cmd):
	#os.system(_cmd)
	#print _cmd
	return

def send_arp():
	shell_cmd('arpMaker')

def made_packet_conf(adapter_name, src, dst, arp_src_mac, arp_src_ip, arp_dst_mac, arp_dst_ip, send_option='no'):
	packet_conf='''ADAPTER_NAME    %s 
ETHER_SRC       %s 
ETHER_DST       %s 
OP_CODE         ARP_REQUEST 
ARP_SRC_MAC     %s 
ARP_SRC_IP      %s 
ARP_DST_MAC     %s 
ARP_DST_IP      %s 
'''%(adapter_name, src, dst, arp_src_mac, arp_src_ip, arp_dst_mac, arp_dst_ip)

	shell_cmd('echo %s > ./packet.conf'%packet_conf)
	
	if(send_option.lower() == 'yes' or send_option.lower() == 'send'):
		send_arp()



def ip_detect(probe_eth, detect_ip, detect_mac=ZERO_MAC, op='broadcast'):
	if 'broadcast' == op.lower():
		made_packet_conf(probe_eth, PROBE_MAC, BROADCAST_MAC, PROBE_MAC,  PROBE_IP, ZERO_MAC, detect_ip, 'yes')
		return True

	if 'unicast' == op.lower():
		made_packet_conf(probe_eth, PROBE_MAC, BROADCAST_MAC, PROBE_MAC,  PROBE_IP, detect_mac, detect_ip, 'yes')
		return True

	return False


for lan in LAN_RANGE:
	for i in range(1, 255):
		ip='%s%s'%(lan,i)
		ip_detect(ETH, ip)
		print 'send to %s'%ip








