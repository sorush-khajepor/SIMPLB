#include <iostream>
#include "lattice.h"
using namespace std;


class TwoPoints{

    protected:
    intND point0;
    intND point1;

    public:

    TwoPoints(){
           point0=0;
           point1=0;
    }

    TwoPoints(const intND& point0_, const intND& point1_){
        set(point0_,point1_);
    }

    TwoPoints(const int& iX0,const int& iY0,const int& iX1,const int& iY1){
      set(iX0,iY0,iX1,iY1);
    }

    TwoPoints(const int& iX0,const int& iY0,const int& iZ0,const int& iX1,const int& iY1,const int& iZ1 ){
      set(iX0,iY0,iZ0,iX1,iY1,iZ1);
    }

    TwoPoints& operator=(const TwoPoints& rhs){
            point0=rhs.point0;
            point1=rhs.point1;
    }

    void set (const intND& point0_, const intND& point1_){
            point0=point0_;
            point1=point1_;
    }

    // Setting 2D points
    void set (const int& iX0,const int& iY0,const int& iX1,const int& iY1){
       point0[0]=iX0;
       point0[1]=iY0;

       point1[0] = iX1;
       point1[1] = iY1;
    }

    // Setting 3D points
    void set (const int& iX0,const int& iY0,const int& iZ0,const int& iX1,const int& iY1,const int& iZ1){
       point0[0]=iX0;
       point0[1]=iY0;
       point0[2]=iZ0;

       point1[0] = iX1;
       point1[1] = iY1;
       point1[2] = iZ1;
    }
    
};

class Box :public TwoPoints{
	public:
	// Get origin of of the box
	const int& getOrigin(const int& iD) const {return point0[iD];}
	// Get dimensions of the box     	
	const int& getDim(const int& iD) const {return point1[iD];}
   	// Volume in 2D is surface area of the box
   	int getVol(){
        	int vol=1;
        	for (int iD=0;iD<lattice::nD;++iD){
            	vol*=getDim(iD);
        }
        return vol;
	}
	void print(){
	for (int iD=0;iD<lattice::nD;++iD){
	    cout<<" Axis="<<iD<<" Origin ="<<getOrigin(iD)<<" Dimension="<<getDim(iD)<<endl;
	}
	cout<<" Volume="<<getVol()<<endl;
	}
};

class LoopLimit :public TwoPoints{
	public:
    	// Begining of the loop over the box, or box origin
	const int& getBegin(const int& iD)const {return point0[iD];}
	// End of the loop over the box
	const int& getEnd  (const int& iD)const {return point1[iD];}
	// Get length of a dimension
	int getDim(const int& iD){return point1[iD]-point0[iD];}
   	// Volume in 2D is surface area of the box
   	int getVol(){
        	int vol=1;
        	for (int iD=0;iD<lattice::nD;++iD){
            	vol*=getDim(iD);
        	}
        	return vol;
	}
	void print(){
	for (int iD=0;iD<lattice::nD;++iD){
	    cout<<"Axis="<<iD<<" Begin ="<<point0[iD]<<" End ="<<point1[iD]<<" Length="<<getDim(iD)<<endl;
	}
	cout<<" Volume="<<getVol()<<endl;
	}
};

int getDotProduct(const int vec1[],const int vec2[]){
	int sum=0.;
	for (int iD=0;iD<lattice::nD;++iD){
		sum += vec1[iD]*vec2[iD];
	}
	return sum;
}

double getDotProduct(const double vec1[],const double vec2[]){
	double sum=0.;
	for (int iD=0;iD<lattice::nD;++iD){
		sum += vec1[iD]*vec2[iD];
	}
	return sum;
}

double getDotProduct(const int vec1[],const double vec2[]){
	double sum=0.;
	for (int iD=0;iD<lattice::nD;++iD){
		sum += vec1[iD]*vec2[iD];
	}
	return sum;
}

