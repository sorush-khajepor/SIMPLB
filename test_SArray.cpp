#include"sArray.h"
typedef SArray<double,2> doubleND;
typedef SArray<int,2> intND;

int main() {

    std::cout<<"===========Testing SArray=============="<<std::endl;
    doubleND a;
    a = 1.10;

    a.print();
    std::cout<<a.sum()<<std::endl;


    (a*2*a).print();
    std::cout<<"===========Testing ArrayND=============="<<std::endl;
    doubleND c=4.1;
    std::cout<<"print scalar constructor "<<std::endl;
    c.print();
    std::cout<<"print scalar assignment "<<std::endl;
    c=2.1;
    c.print();

    std::cout<<"print + operator "<<std::endl;
    (c+2.1).print();

    std::cout<<"print sum funtion "<<std::endl;
    c = c+2.1;
    std::cout<<"sum="<<c.sum()<<std::endl;

    std::cout<<"print array constructor "<<std::endl;
    double arr[2]={1.5,2.0};
    doubleND d(arr);
    d.print();

    std::cout<<"print list constructor"<<std::endl;
    intND g={1,2};
    g.print();

    std::cout<<"print dot product (int=>double)"<<std::endl;
    std::cout<<d.dot(g)<<std::endl;

    std::cout<<"print dot product (double=>int)"<<std::endl;
    std::cout<<g.dot(d)<<std::endl;

    std::cout<<"print int-double-number, type conversion operation"<<std::endl;
    c = (doubleND)g*2+d;
    c.print();

    return 0;
}
