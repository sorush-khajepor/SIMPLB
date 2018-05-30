#include "node.h"

int main(int argc, char const *argv[]) {
    Node n,m;

    cout<<"Set to something n:";
    n[0]=0;
    n[1]=1;
    n[2]=2;
    n[3]=3;
    n[4]=4;
    n[5]=5;
    n[6]=6;
    n[7]=7;
    n[8]=8;

    cout<<n;

    cout<<"mirrored n:"    ;
    n.mirror();
    cout<<n;

    cout<<"m = n";
    m=n;
    cout<<m;

    cout<<"array assignment m=arr";
    double arr[lattice::nQ]={8.1,7.1,6.1,5.1,4.1,3.1,2.1,1.1,0.1};
    m=arr;
    cout<<m;
    return 0;
}
