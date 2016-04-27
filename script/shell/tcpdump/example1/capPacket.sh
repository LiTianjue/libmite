#!/bin/bash

# $1 监视的设备
# $2 监视的端口

one=1
two=2
while (( $one != $two ))
do
	sleep 1
	#TIME=`date +%F"-"%H:%M:%S`
	TIME=`date +%H%M%S`

	#tcpdump -i eth0 -s 0 -w tcpdump.$TIME &
	tcpdump -i $1 port $2  -s 0 -w $1_$2_$TIME.pcap &
	sleep 20
	ps -ef | grep tcpdump | grep -v grep | awk '{print $2}' | xargs kill -9
done
