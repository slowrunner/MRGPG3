#!/bin/bash

echo "Routine Shutdown Requested"
homedir="/home/tovli/MRGPG3"
batt=`($homedir/plib/battery.py)`
$homedir/utils/logMaintenance.py "Routine Shutdown"
$homedir/utils/logMaintenance.py "'$batt'"
sudo shutdown -h +2
