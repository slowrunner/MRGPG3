# GoPiGo3 C++ API and Examples


This C++ project builds:
- libgopigo3.lib.a:  GoPiGo3 C++ API link library
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
  - testEasyGoPiGo3
  - vbatt

### Structure:  
```
  build/
    lib/    libgopigo3_cpp.a
    bin/    myrobot Example executables
    build/  (CMake temp files - safe to delete)
```

### To build the executables:  ```bash build_gopigo3proj.sh```

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
  and under active testing, debug, and documentation  
     

