# GoPiGo3 C++ API and Examples


This C++ project builds:
- libgopigo3.lib.a:  GoPiGo3 and EasyGoPiGo3 C++ API link library
- SAMPLE ROBOT
  - myrobot:           C++ program to drive the GoPiGo3 using keyboard commands
- API USE EXAMPLES
  - drive
  - grove_led
  - i2c
  - info
  - leds
  - motors
  - sensors
  - servos
  - testEasyGoPiGo3  Used to test all EasyGoPiGo3 C++ API methods
  - vbatt

### Structure:  
```
  build/
    lib/    libgopigo3_cpp.a
    build/  (CMake temp files - safe to delete)
    build/bin/    myrobot and Examples/ executables
```

### To build the executables:  
```
bash build_gopigo3proj.sh
or
./build_gopigo3proj.sh
```
NOTE:  GoPiGo3.cpp will throw some warnings - don't know how to eliminate properly.


To run the executables:
```
build/bin/name_of_executable
```
or
```
cd build/bin
./name_of_executable   e.g.  ./myrobot
```

To clean all build products:
```
rm -rf build
or 
./clean.sh
```

# NOTE about GoPiGo3 C++ API EasyGoPiGo3:  

Dexter Industries and later Modular Robotics have never released a GoPiGo3 C++ EasyGoPiGo3 class.  
My gopigo3proj contains:  
- My "fixes" to the New Install GoPiGo3 C++ API (pull request in place)
- EasyGoPiGo3.cpp and .h prototypes that were trans-coded from EasyGoPiGo3.py by Claude Code  
  and passed testing by Examples/testEasyGoPiGo3.cpp (co-written with Claude by slowrunner)  
     


### To run Examples (after make):    
- ```build/bin/info```        - reads GoPiGo3 red board information and reports battery voltage    
- ```build/bin/vbatt```       - reports battery voltage reading and VCC (5v) voltage     
                                Actual battery voltage is 0.8v higher than reported    
                                due to reverse polarity protection diode drop.  
- ```build/bin/leds```        - cycles center "WiFi LED" through colors  
- ```build/bin/servos```      - centers properly attached servos  
                                (Servo1: brown wire closest to front of robot)  
                                (Servo2: brown wire closest to back of robot)  
- ```build/bin/motors```      - Gently rotate robot's left wheel in a forward direction    
                                Robot's right wheel will rotate to match left wheel encoder value  
- ```build/bin/sensors```     - Reports Grove Ultrasonic Ranger (connected to AD1) range in millimeters  
                                Reports Infrared Remote Control (connected to AD2) code  
- ```build/bin/i2c```         - Toggles P0 output of PCA9570 I2C output expander connected to AD1 port ??  ?  
- ```build/bin/grove_led```   - Varies Grove LED brightness (connected to AD1)  
- ```build/bin/drive```       - Allows driving GoPiGo3 with key presses  
                                spacebar: stop  
                                w: forward at 150 DPS  
                                s: spin  
                                x: backward at 150 DPS  
                                a: pivot ccw around left wheel  
                                d: pivot cw around right wheel  
                                q: exit (only q, cntrl-c does not exit)  
- ```build/bin/myrobot```       - runs test robot (drive.cpp with status output if press key during motion)    
