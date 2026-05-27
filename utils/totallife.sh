#!/bin/bash
#
# totallife.sh    print total hours and sessions of life in life.log
#
# requires bc  (sudo apt-get install bc)
#
# Counted Keys:
#   Playtimes:           ": success "
#   Sessions:            "- boot -"              ( "\- boot \-" search string )
#   Safety shutdowns:    "SAFETY SHUTDOWN"
#   failure
#
# Summed Keys:
#   Undocking, ... after __
#   execution prior:
#   execution:
#   prior dockings:
#   h playtime
#   h charging
#   moved:          (odometer.log)

# declare -i newBatteryAtCycle=416
declare -i newBatteryAtCycle=2159
reborn="25 May 2026"
homedir='/home/tovli/MRGPG3'

echo "(Cleaning life.log first)"
/home/tovli/MRGPG3/plib/cleanlifelog.py
echo " "
fn="${homedir}/logs/life.log"
# echo -e "fn: $fn"
ofn="${homedir}/logs/odometer.log"
priorLife=`(awk -F'execution prior:' '{sum+=$2}END{print sum;}' $fn)`
priorDockings=`(awk -F'prior dockings:' '{sum+=$2}END{print sum;}' $fn)`
totalAwake=`(awk -F'execution:' '{sum+=$2}END{print sum;}' $fn)`
totalNaps=`(awk -F'nap for' '{sum+=$2}END{print sum;}' $fn)`
totalThisLife=`(echo "scale=1; ($totalAwake + $totalNaps)" | bc)`
totalLife=`(echo "scale=1; ($totalAwake + $totalNaps + $priorLife)" | bc)`
# weirdness without +'' at end, see ls appended to lastDockingStr??
lastDockingStr=`(grep "h playtime" $fn | tail -1 )`+''
# echo "lastDockingStr: " $lastDockingStr " :"
lastUndockingStr=`(grep "h charging" $fn | tail -1)`
totalDockings=`(awk -F"Docking " '{sub(/ .*/,"",$2);print $2}' <<< $lastDockingStr)`
dockingsThisLife=`(echo "scale=1; $totalDockings - $priorDockings" | bc)`
currentBattCycles=`(echo "scale=1; $totalDockings - $newBatteryAtCycle" | bc)`

echo "*** MRGPG3 Dave TOTAL LIFE STATISTICS ***"
# echo "Total Awake:  " $totalAwake " hrs"
# echo "Total Naps:    " $totalNaps " hrs"
echo "This Life:   " $totalThisLife " hrs (since $reborn)"
echo "Total Life:   " $totalLife " hrs (since 21 June 2021)"
echo "Kilted-Dave Playtimes (Undocked-Docked):" `(grep -c ": success" $fn)`
echo "Dockings This Life: " $dockingsThisLife
echo "Total Dockings: " $totalDockings
echo "New Battery Installed At Docking:" $newBatteryAtCycle
echo "This Battery At Cycle: " $currentBattCycles
last3playtimes=`(grep " h playtime" $fn | tail -3 | awk -F" after "  '{sum+=$2}END{print sum;}' )`
last3avePlaytime=`(echo "scale=1; $last3playtimes / 3" | bc)`
echo "Average playtime (last three)" $last3avePlaytime "hrs "
last3dockedtimes=`(grep "Undocking, " $fn | tail -3 | awk -F" after "  '{sum+=$2}END{print sum;}' )`
last3aveDockedtime=`(echo "scale=1; $last3dockedtimes / 3" | bc)`
echo "Average docked time (last three)" $last3aveDockedtime "hrs "
booted=`(grep -c "\- boot \-" $fn)`
echo "Sessions (boot): " `(grep -c "\- boot \-" $fn)`
aveSession=`(echo "scale=1; ($totalThisLife / $booted)" | bc -l)`
echo "Average Session: " $aveSession "hrs"
safetyShutdowns=`(grep -c "SAFETY SHUTDOWN" $fn)`
echo "Safety Shutdowns: " $safetyShutdowns 
dockingFailures=`(grep -c "Docking : failure" $fn)`
echo "Docking Failures: " $dockingFailures 
totalMoved=`(awk -F'moved:' '{sum+=$2}END{printf "%.1f", sum;}' $ofn)`
totalMovedFt=`(echo "scale=1; ($totalMoved / 0.3048)" | bc)`
echo "Total Travel: " $totalMoved "meters" $totalMovedFt "feet"
echo " "
echo "Last Undocking String: " $lastUndockingStr
echo "Last Docking   String: " $lastDockingStr
