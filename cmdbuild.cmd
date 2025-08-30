cmake -E make_directory build
cmake -E chdir build cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ../
cmake --build build