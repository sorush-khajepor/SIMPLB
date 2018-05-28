#include "lattice.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {

    cout<< "Qvector"<<endl;
    cout<<lattice::Qvector;

    cout<< "iOpposite"<<endl;
    cout<<lattice::iOpposite;

    cout<< "iHalfQs"<<endl;
    cout<<lattice::iHalfQs;
    
    cout<< "weight"<<endl;
    cout<<lattice::weight;
    return 0;
}
