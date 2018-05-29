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

   	// Volume in 2D is surface area of the box
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

    LoopLimit& operator=(const LoopLimit& rhs){
            begin=rhs.begin;
            end=rhs.end;
    }

    LoopLimit(std::initializer_list<int> begin_,std::initializer_list<int> end_) {

        begin=begin_;
        end=end_;
    }

    // Begining of the loop over the box, or box origin
	const intND& getBegin()const {return begin;}
	const int& getBegin(const int& iD)const {return begin[iD];}

	// End of the loop over the box
	const intND& getEnd()const {return end;}
	const int& getEnd  (const int& iD)const {return end[iD];}

	// compute length of a dimension
	intND computeDim(){return end-begin;}
	int computeDim(const int& iD){return end[iD]-begin[iD];}

   	// Volume in 2D is surface area
   	int computeVol(){
            	return computeDim().computeVol();
	}
	friend std::ostream& operator<<(std::ostream& os, const LoopLimit& loopLimit){
	            std::cout<<"origin = "<<loopLimit.begin;
	            std::cout<<"dimension ="<<loopLimit.end;
	            return os;
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

