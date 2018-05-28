#include"sArray.h"
typedef SArray<double,2> doubleND;
typedef SArray<int,2> intND;

int main() {

    std::cout<<"===========Testing ArrayND=============="<<std::endl;
    doubleND c=4.1;
    std::cout<<"print scalar constructor c "<<std::endl;
    c.print();
    std::cout<<"print scalar assignment c"<<std::endl;
    c=2.1;
    c.print();

    std::cout<<"print same type constructor b"<<std::endl;
    doubleND b=c;
    b.print();

    std::cout<<"print same type assignment m"<<std::endl;
    doubleND m;
    m=b;
    m.print();

    std::cout<<"print + operator "<<std::endl;
    (c+2.1).print();

    std::cout<<"print sum funtion "<<std::endl;
    c = c+2.1;
    std::cout<<"sum="<<c.sum()<<std::endl;

    std::cout<<"print array constructor d"<<std::endl;
    double arr[2]={1.5,2.0};
    doubleND d(arr);
    d.print();

    std::cout<<"print list constructor g"<<std::endl;
    intND g={1,2};
    g.print();

    std::cout<<"print dot product g.d (int=>double)"<<std::endl;
    std::cout<<d.dot(g)<<std::endl;

    std::cout<<"print dot product d.g (double=>int)"<<std::endl;
    std::cout<<g.dot(d)<<std::endl;

    std::cout<<"print int-double-number, type conversion operation, (doubleND)g*2+d"<<std::endl;
    c = (doubleND)g*2+d;
    c.print();

    return 0;
}
