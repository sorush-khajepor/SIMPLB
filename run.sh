rm *.gch
rm a.out
# For compiling serial
#g++ -std=c++11 main.cpp
#./a.out
# For compiling parallel
mpiCC -std=c++11 main.cpp
mpirun -n 4 ./a.out

