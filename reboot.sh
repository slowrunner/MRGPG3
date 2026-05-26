#!/bin/bash

echo "Routine Reboot Requested"
homedir="/home/tovli/MRGPG3"
batt=`($homedir/plib/battery.py)`
$homedir/utils/logMaintenance.py "Routine Reboot"
$homedir/utils/logMaintenance.py "'$batt'"
sudo reboot
