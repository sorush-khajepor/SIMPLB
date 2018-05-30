#include"sArray.h"
typedef SArray<double,2> doubleND;
typedef SArray<int,2> intND;
using namespace std;
int main() {

    cout<<"===========Testing ArrayND=============="<<endl;
    doubleND c=4.1;
    cout<<"print scalar constructor c "<<endl;
    cout<<c;
    cout<<"print scalar assignment c=2.1"<<endl;
    c=2.1;
    cout<<c;

    cout<<"print same type constructor b"<<endl;
    doubleND b=c;
    cout<<b;

    cout<<"print same type assignment m"<<endl;
    doubleND m;
    m=b;
    cout<<m;

    cout<<"print + operator c+2.1"<<endl;
    cout<<(c+2.1);

    cout<<"print sum funtion "<<endl;
    c = c+2.1;
    cout<<"sum="<<c.sum()<<endl;

    cout<<"print array constructor d"<<endl;
    double arr[2]={1.5,2.0};
    doubleND d(arr);
    cout<<d;

    cout<<"print list constructor g"<<endl;
    intND g={1,2};
    cout<<g;

    cout<<"print dot product g.d (int=>double)"<<endl;
    cout<<d.dot(g)<<endl;

    cout<<"print dot product d.g (double=>int)"<<endl;
    cout<<g.dot(d)<<endl;

    cout<<"print int-double-number, type conversion operation, (doubleND)g*2+d"<<endl;
    c = (doubleND)g*2+d;
    cout<<c;

    return 0;
}
