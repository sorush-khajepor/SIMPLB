#include"container.h"

int main() {

    SArray<double,3> a;
    a = 1.10;

    a.print();
    std::cout<<a.sum()<<std::endl;

    // Type casting
    SArray<int,3>(a).print();


    (a*2*a).print();

/////////////////////////////////
    ArrayND<double> c;
    c=2;
    c.print();

    (c+2.1).print();


    std::cout<<(c+2.5).sum()<<std::endl;

    double arr[2]={1,5};
    ArrayND<double> d;
    d = arr;
    d.print();

    std::cout<<c.dot(d)<<std::endl;


    return 0;
}
