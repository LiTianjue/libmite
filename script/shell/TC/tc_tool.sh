#!/bin/bash

# $1 command :  add ,del ,show
# $2 网络延时 100ms
# $3 抖动 10ms
# $4 抖动概率 30%
# $5 网络丢包 10%
# $6 数据包损坏 0.5%

if [[ $1 == "add" ]]
then
	echo "add..."
	tc qdisc add dev eth0 root netem delay $2 $3 $4 loss $5 
elif [[ $1 == "del" ]]
then
	echo "del..."
	tc qdisc del dev eth0 root netem
elif [[ $1 == "show" ]]
then
	echo "show:"
	tc qdisc show dev eth0
else
	echo "Unknow cmd"
	echo "$0 add/del/show"
fi
