#!/bin/bash
rfkill unblock wifi
ip link set wlan0 up
wpa_supplicant -B -i wlan0 -c ./wpa.conf
sleep 1
ls /run/wpa_supplicant/
