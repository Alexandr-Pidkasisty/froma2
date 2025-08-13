mkdir build
cd build
set PATH=C:\C++\pocketcpp\MinGW\bin;%PATH%
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ../
mingw32-make.exe