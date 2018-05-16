rm *.gch
rm a.out
# For compiling serial
#g++ -std=c++11 main.cpp
# For compiling parallel
mpiCC -std=c++11 main.cpp
./a.out

