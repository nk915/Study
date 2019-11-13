#!/bin/bash
# pid 생성 을 해서 중복실행을 방지하는 코드 - made by Enteroa (san0123a@naver.com)
if [[ -s $0.pid ]];then exist_pid=`cat $0.pid`
	if [[ -z `ps -e|grep "^$exist_pid "` ]];then rm -f $0.pid;exec_confirm="Y"
    else exec_confirm="N";echo -e "\e[1;32m쉘스크립트가 이미 실행중입니다.Shell has already running...\e[0m";fi
else exec_confirm="Y"
fi

if [[ $exec_confirm == "Y" ]];then
	echo $$ > $0.pid
##############################################################################
# 기존 쉘 입력
#############################################################################
	rm -f $0.pid
fi
