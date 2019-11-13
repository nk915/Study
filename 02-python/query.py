#!/usr/bin/python
#-*-coding:utf-8
import socket, struct

SQL_FILE_NAME = "IPList.sql"
INSERT_QUERY = """\
INSERT INTO T_Agent_Detected_IP ( SCTime, DTime, StateChangeTime, IP, MACU, MACL,  Name, NBGroup, AgentFlag, State, GroupID,\
AgentMACU, AgentMACL, AdaptorName ) VALUES ( 0, 0, 0, %s, 0, 0, '_N_U_L_L', '_N_U_L_L', 0, 0, 0, \
2863311530, 426, 'eth0' );
"""
INSERT_IP = "172.31.%d.%d"


def ip2long(ip):
	packedIP = socket.inet_aton(ip)
	return struct.unpack("!L", packedIP)[0]

# 1. file open
fd = open(SQL_FILE_NAME, 'w')

# 2. write query into sql file
cnt=0
for i in range(221,255):
	for j in range(1,255):
		cnt+=1
		#print INSERT_QUERY%(ip2long(INSERT_IP%(i,j)))
		query = INSERT_QUERY%(ip2long(INSERT_IP%(i,j)))
		fd.write(query)

fd.close()
print "Total : " + str(cnt)
