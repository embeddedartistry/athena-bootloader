#!/bin/bash
eth_dev="eth0"
eth_ip="192.168.2.1"
ard_ip="192.168.2.250"

if [ "${1}" = "-s" ]
then
	sudo ifconfig ${eth_dev} down
	sudo ifconfig ${eth_dev} ${eth_ip} netmask 255.255.255.0
	sudo route add ${ard_ip} gw ${eth_ip} dev ${eth_dev}
	
	echo "Network settings have been set"
	sleep 2
	ping -c4 ${ard_ip}

elif [ "${1}" = "-u" ]
then
	sudo ifcfg ${eth_dev} stop
	sudo ifconfig ${eth_dev} down
	echo "Network settings have been unset"

else
	echo "Unknown option"
	echo "Use -s to set or -u to unset the network settings"
fi
