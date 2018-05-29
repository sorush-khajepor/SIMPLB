#include "geometryTools.h"
using namespace std;
int main (){
    
    cout<<" Box test:"<<endl;
    Box box({0,1},{6,7}) ;

    std::cout<<box;

    std::cout<<box.computeVol()<<endl;


    cout<<" LoopLimit test:"<<endl;
    LoopLimit ll({0,1},{6,7}) ;

    std::cout<<ll;

    std::cout<<"volume="<<ll.computeVol()<<endl;
    std::cout<<"dimension="<<ll.computeDim()<<endl;

    return 0;
}
