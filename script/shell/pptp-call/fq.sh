#!/bin/bash

VPN_NAME=myvpn
VPN_SERVER=23.108.173.3
VPN_USER=mhpmii
VPN_PASSWORD=China2008

# install
if [[ $1 == "install" ]]
then
	echo "Install"
	apt-get install pptp-linux
fi

if [[ $1 == "create" ]]
then
	# create a vpn in /etc/ppp/peers/
	echo "Create"
	pptpsetup -create ${VPN_NAME} -server ${VPN_SERVER} -username ${VPN_USER} -password ${VPN_PASSWORD}
fi

if [[ $1 == "on" ]]
then
	echo "VPN ON"
	pon ${VPN_NAME}
	sleep 1
	#route add -net 0.0.0.0 netmask 0.0.0.0 dev ppp0

	#route del default
	route add default dev ppp0
fi

if [[ $1 == "off" ]]
then
	echo "VPN OFF"
	route del -net 0.0.0.0 netmask 0.0.0.0 dev ppp0
	poff ${VPN_NAME}
fi
