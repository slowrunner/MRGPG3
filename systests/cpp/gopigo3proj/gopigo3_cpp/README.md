# The GoPiGo3 C++ API


In the New Trixie_Install.sh (Option B)  
```
  /GoPiGo3/Software/C/
``` 
contains the latest official GoPiGo3 C++ API in files GoPiGo3.h and GoPiGo3.cpp 
(which end up as the .h and libgopigo3.so for linking to user C++ programs)  

This folder is the root for building the GoPiGo3 C++ API with a more traditional C++ structure:
- src      Contains GoPiGo3.cpp (and prototype EasyGoPiGo3.cpp)
- include  Contains GoPiGo3.h   (and prototype EasyGoPiGo3.h  )
- CMakeLists.txt  Tells cmake how to build the GoPiGo3 C++ API



