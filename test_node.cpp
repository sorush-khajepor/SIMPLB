#include "node.h"

int main(int argc, char const *argv[]) {
    Node n,m;

    n = 9.;

    n[0]=0;
    n[1]=1;
    n[2]=2;
    n[5]=5;
    n[6]=6;


    cout<<n;
    n.mirror();
    cout<<n;

    m=n;
    cout<<m;
    return 0;
}
