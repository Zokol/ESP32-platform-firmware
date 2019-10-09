#!/bin/bash
############################
# Badge.team Flash-O-Matic #
############################
#
# Author: Heikki Juva (heikki@juva.lu)
# Version: 0.1
# Date: 09.10.2019
#
# Description:
# Intended to be used in headless flashing rig, requires no user interaction
# Adds udev-rule to detect badge, create symlink and run flashing script
# 
# ToDo:
# - testing
# - detection of badge type and automatic binary file selection

apt update && apt install -y python
pip install esptool

cat << EOF >> /opt/badgeteam/autoflash.sh
#!/bin/sh
esptool.py --chip esp32 --port /dev/badgeteam --baud 460800 write_flash -z 0x1000 firmware.bin
EOF
chmod +x /opt/badgeteam/autoflash.sh

rm -f /etc/udev/rules.d/90-badgeteam.rules 
cat << EOF >> /etc/udev/rules.d/90-badgeteam.rules
SUBSYSTEM=="tty", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", SYMLINK+="badgeteam", RUN+="/opt/badgeteam/autoflash.sh"
EOF
sudo /etc/init.d/udev restart
