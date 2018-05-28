#include"container.h"

int main() {

    SArray<double,3> a;
    a = 1.10;

    a.print();
    std::cout<<a.sum()<<std::endl;

    SArray<int,3>(a).print();

    std::cout<<SArray<int,3>(a).sum()<<std::endl;
    return 0;
}
