#!/bin/bash
#
# wheelstats.sh    print total travel, rotation, travel time and segments of travel in wheel.log
#
# requires bc  (sudo apt-get install bc)
#
# Note: bc scale=x only works for division!
#
echo "WHEEL STATS"
fn="/home/tovli/MRGPG3/logs/wheel.log"

echo "(Cleaning life.log first)"
/home/tovli/MRGPG3/plib/cleanlifelog.py
echo " "
declare -i catchupSegmentss=0
totalTravel=`(awk -F'travel:' '{sum+=sqrt($2^2)}END{printf "%.1f", sum/1000;}' $fn)`
totalTravelFt=`(echo "scale=1; ($totalTravel / 0.3048)" | bc)`
echo "Total Travel: " $totalTravel "m" $totalTravelFt "ft"
totalRotate=`(awk -F'rotation:' '{sum+=sqrt($2^2)}END{printf "%.1f", sum;}' $fn)`
totalRevs=`(echo "scale=1; $totalRotate / 360" | bc)`
echo "Total Rotate: " $totalRotate "deg" $totalRevs "revolutions"
totalMotion=`(awk -F'motion:' '{sum+=$2}END{printf "%.1f", sum;}' $fn)`
totalMotionHrs=`(echo "scale=3; $totalMotion / 3600" | bc)`
echo "Total Motion: " $totalMotion "sec" $totalMotionHrs "hrs"
totalLife=`(awk -F':' '{sum+=$3}END{printf "%.1f", sum;}' /home/tovli/MRGPG3/logs/life.log)`
percentInMotion=`(echo "scale=2; $totalMotionHrs * 100.0 / $totalLife" | bc)`
echo "Total Life: " $totalLife "hrs   percentInMotion:" $percentInMotion
