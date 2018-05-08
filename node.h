
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

	double& operator[] (const int& q){
		return f[q];	
	}

    Node& operator= (const double& rhs){

		for (int iQ=0;iQ<lattice::nQ;iQ++){
			f[iQ] = rhs;
		}
    }

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
		double f_tmp;
		int q;
		for (vector<int>::const_iterator it=halfQs.begin();it!=halfQs.end();++it){
			q = *it;
			std::swap(f[q],f[opposite[q]]);
		}
	}

  //  void revStream() {


   // }

	void print () {
		for (int iQ=0;iQ<lattice::nQ;iQ++){
			cout<< "f("<<iQ<<") = "<<f[iQ]<< endl;
		}
	}

};
