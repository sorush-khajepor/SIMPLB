#include "lattice.h"
#include <iostream>
#include <vector>

using namespace std;

class Node {

protected:	

    // Particle distibution functions (distros)
    double f[lattice::nQ];

public:

    Node() {
        *this= (double) 0.;
    }

    // Altering access
    double& operator[] (const int& q){return f[q];}

    // Constant access
    double const& operator[] (const int& q) const {return f[q];}

    // Set all distros to a number
    Node& operator= (const double& rhs){
        for (int iQ=0;iQ<lattice::nQ;iQ++){
            f[iQ] = rhs;
        }
    }

    // Set node with another node
    Node& operator= (const Node& rhs){

        for (int iQ=0;iQ<lattice::nQ;iQ++){
            f[iQ] = rhs.f[iQ];
        }
    }


    void mirror () {
        int iQ;
        for (int i=0;i<lattice::nQ/2;++i){
            iQ = lattice::iHalfQs[i];
            std::swap(f[iQ],f[lattice::iOpposite[iQ]]);
        }
    }


    void print () {
        for (int iQ=0;iQ<lattice::nQ;iQ++){
            cout<< "f("<<iQ<<") = "<<f[iQ]<< endl;
        }
    }

};
