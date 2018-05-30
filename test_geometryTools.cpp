#include "geometryTools.h"
using namespace std;
int main (){
    
    cout<<" Box test:"<<endl;
    Box box({0,1},{6,7}) ;

    cout<<box;

    cout<<box.computeVol()<<endl;


    cout<<" LoopLimit test:"<<endl;
    LoopLimit ll({0,1},{6,7}) ;

    cout<<ll;

    cout<<"volume="<<ll.computeVol()<<endl;
    cout<<"dimension="<<ll.computeDim()<<endl;

    return 0;
}
