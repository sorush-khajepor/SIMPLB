#include <iostream>
#include "domain.h"
using namespace std;


int main () {
	
	Node a;
	cout.precision(18);

	for (int q=0;q<lattice::nQ;q++){
		a[q] = q;
		cout<< a[q]<< endl;
	};			
    a.mirror();

	for (int q=0;q<lattice::nQ;q++){
	    cout<< a[q]<< endl;
	};

	Domain domain(6,5);

/*	domain(0,0) = 6;
	domain(0,0).print();

	cout<<"this is = "<<domain(0,0)[1]<<endl;

	domain.print();
	int ixn =0;
	int iyn =0;

   for (int q=0;q<nQ;++q){
	domain.getPeriodicNeighbor(3,3,q,ixn,iyn);
	cout<<ixn<<","<<iyn<<endl;
   }
   */
	domain(3,3)[1] = 1.;
	for (int t=0;t<10;++t){
		domain.revStream();
		domain.mirror();
        for (int iX=0;iX<domain.getNX();++iX){
            for (int iY=0;iY<domain.getNY();++iY){
            	for(int iQ=0;iQ<nQ;++iQ){
                	if(domain(iX,iY)[iQ]>0.00001){cout<<iX<<","<<iY<<","<<iQ<<"="<<domain(iX,iY)[iQ]<<endl;}
            	}
            }
        }
	}
return 0;	
}
