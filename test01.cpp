#include"container.h"

int main() {

    SArray<double,3> a;
    a = 1.10;

    a.print();

    SArray<int,3> b;

    SArray<int,3>(a).print();


    return 0;
}
