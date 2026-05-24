# GoPiGo3 in C++

There are two folders:  
```
- NewGoPiGo3_Software_C  which matches the new install Option B to $HOME/GoPiGo3/Software/C
                           - with mod for get_motor_encoders() to properly return encoder value as int32_t
                           - with mod for GPG.MOTOR_TICKS_PER_DEGREE as float to hold 16 tick value of 5.333
                           - with mod making header defined vars and function as static to prevent duplicate error
  
- gopigo3proj            which is a more typical C++ project structure  
                           - with gopigo3_cpp building the gopigo3 link library for GoPiGo3 and EasyGoPiGo3 C++ API  
                           - Examples building executables that use the GoPiGo3 and EasyGoPiGo3 C++ APIs  
                           - robot building "myrobot" executable similar to Examples/drive.cpp  
```

NOTE: EasyGoPiGo3 C++ API only exists in gopigo3proj
