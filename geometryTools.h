#include "lattice.h"
#include <iostream>


class Box {
    intND origin;
    intND dim;

	public:

    Box(){
        origin =0;
        dim =0;
    }

    Box(const intND& origin_, const intND& dim_): origin(origin_),dim(dim_){};

    Box& operator=(const Box& rhs){
            origin=rhs.origin;
            dim=rhs.dim;
    }

    Box(std::initializer_list<int> origin_,std::initializer_list<int> dim_) {

        origin=origin_;
        dim = dim_;
    }

	// Get origin of of the box
	const intND& getOrigin() const {return origin;}
	const int& getOrigin(const int& iD) const {return origin[iD];}

	// Get dimensions of the box     	
	const intND& getDim() const {return dim;}
	const int& getDim(const int& iD) const {return dim[iD];}

   	// Compute volume (in 2D volume is surface area of the box)
   	int computeVol(){
         return	dim.computeVol();
	}

	friend std::ostream& operator<<(std::ostream& os, const Box& box){
	            std::cout<<"origin = "<<box.origin;
	            std::cout<<"dimension ="<<box.dim;
	            return os;
	        }
};

class LoopLimit {
    intND begin;
    intND end;

	public:

    LoopLimit(){
        begin =0;
        end =0;
    }

    LoopLimit(const intND& begin_, const intND& end_): begin(begin_),end(end_){};

    void set(const intND& begin_, const intND& end_){
        begin = begin_;
        end   = end_;
    }

    LoopLimit& operator=(const LoopLimit& rhs){
            begin=rhs.begin;
            end=rhs.end;
    }

    LoopLimit(std::initializer_list<int> begin_,std::initializer_list<int> end_) {

        begin=begin_;
        end=end_;
    }

    // Beginning of the loop over the box, or box origin
	const intND& getBegin()const {return begin;}
	const int& getBegin(const int& iD)const {return begin[iD];}

	// End of the loop over the box
	const intND& getEnd()const {return end;}
	const int& getEnd  (const int& iD)const {return end[iD];}

	// Compute length of a dimension
	intND computeDim(){return end-begin;}
	int computeDim(const int& iD){return end[iD]-begin[iD];}

   	// Compute volume (in 2D volume is surface area)
   	int computeVol(){
            	return computeDim().computeVol();
	}
	friend std::ostream& operator<<(std::ostream& os, const LoopLimit& loopLimit){
	            std::cout<<"begin = "<<loopLimit.begin;
	            std::cout<<"end-1 ="<<loopLimit.end-1;
	            return os;
	        }
};

typedef SArrayBase<LoopLimit,lattice::nQ> LoopLimitNQ;

void shrink(LoopLimitNQ& reference,LoopLimitNQ& shrunk, int level=1){

    for (int iQ=0;iQ<lattice::nQ;iQ++){
        intND begin,end;
        for (int iD=0;iD<lattice::nD;iD++){
            if (lattice::Qvector[iQ][iD] != 0 ){
                begin[iD] = reference[iQ].getBegin(iD)-level*lattice::Qvector[iQ][iD];
                end  [iD] = reference[iQ].getEnd  (iD)-level*lattice::Qvector[iQ][iD];
            }else{

                begin[iD] = reference[iQ].getBegin(iD)+level;
                end  [iD] = reference[iQ].getEnd  (iD)-level;
            }
            shrunk[iQ].set(begin,end);
        }
    }
};


// Get dot product of two vectors (i.e. a.b = a1b1 + a2b2 + ... + anbn)
// Dot product of two integers = integer
int getDotProduct(const int vec1[],const int vec2[]){
	int sum=0.;
	for (int iD=0;iD<lattice::nD;++iD){
		sum += vec1[iD]*vec2[iD];
	}
	return sum;
}
// Dot product of two floating point vectors = double
double getDotProduct(const double vec1[],const double vec2[]){
	double sum=0.;
	for (int iD=0;iD<lattice::nD;++iD){
		sum += vec1[iD]*vec2[iD];
	}
	return sum;
}
// Dot product of integer and floating point vectors = double
// Note: take care with order of int and double
double getDotProduct(const int vec1[],const double vec2[]){
	double sum=0.;
	for (int iD=0;iD<lattice::nD;++iD){
		sum += vec1[iD]*vec2[iD];
	}
	return sum;
}

