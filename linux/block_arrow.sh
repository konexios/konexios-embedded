#!/bin/sh

PORT=$(netstat -punta | grep arrow | awk '{print $4}' | sed 's/.*://')

iptables -I INPUT -p tcp --dport $PORT -j DROP ; iptables -I OUTPUT -p tcp --sport $PORT -j DROP

echo "block port $PORT"

sleep 3

killall -9 arrow.bin

read -p "Press enter to unlock"

iptables -D INPUT -p tcp --dport $PORT -j DROP ; iptables -D OUTPUT -p tcp --sport $PORT -j DROP
