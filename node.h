#include "lattice.h"
#include <iostream>
using namespace std;


class Node {

protected:

    // Particle distibution functions (distros)
    doubleNQ* f;

    // Node Collision
    // Collision* collision;

public:

    Node() {
        f=new doubleNQ (0.);
    }

    doubleNQ& getF() {return *f;}
    const doubleNQ& getF() const {return *f;}

    // Read-Write access
    double& operator[] (const int& q){return (*f)[q];}

    // Read-Only access
    double const& operator[] (const int& q) const {return (*f)[q];}

    // Set all distros to a number
    Node& operator= (const double& rhs){
        *f=rhs;
    }

    Node& operator= (const double rhs[]){
        *f=rhs;
    }

    // Set node with another node
    Node& operator= (const Node& rhs){
            *f = *(rhs.f);
    }


    void mirror () {
        for (int i=0;i<lattice::nQ/2;++i){
            int iQ = lattice::iHalfQs[i];
            std::swap((*f)[iQ],(*f)[lattice::iOpposite[iQ]]);
        }
    }


    friend std::ostream& operator<<(std::ostream& os, const Node& node){
        cout<<*(node.f);
        return os;
    }
};
