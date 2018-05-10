
#include "lattice.h"
#include <iostream>
#include <vector>

using namespace lattice;
using namespace std;

class Node {

protected:	

	double f[nQ];
	
	
public:

	Node() {set( (double) 0.); }

    // Altering access
	double& operator[] (const int& q){return f[q];}

	// Constant access
	double const& operator[] (const int& q) const {return f[q];}

	// Set all distros to a number
    Node& operator= (const double& rhs){
    	set(rhs);
    }

    // Set node with another node
    Node& operator= (const Node& rhs){

		for (int iQ=0;iQ<lattice::nQ;iQ++){
			f[iQ] = rhs.f[iQ];
		}
    }

    void set(const double& rhs){

		for (int iQ=0;iQ<lattice::nQ;iQ++){
			f[iQ] = rhs;
		}
    }

	void mirror () {
		int iQ;
		for (int i=0;i<nQ/2;++i){
			iQ = iHalfQs[i];
			std::swap(f[iQ],f[iOpposite[iQ]]);
		}
	}


	void print () {
		for (int iQ=0;iQ<lattice::nQ;iQ++){
			cout<< "f("<<iQ<<") = "<<f[iQ]<< endl;
		}
	}

};
