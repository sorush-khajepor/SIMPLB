#include <iostream>
#include "lattice.h"
using namespace std;


class Box{

    int begin[lattice::nD];
    int end[lattice::nD];

public:

    Box(){
       for (int iD=0;iD<lattice::nD;++iD){
           begin[iD]=0;
           end[iD] = 0;
       }
    }

    Box(const int& iXbegin,const int& iYbegin,const int& iXend,const int& iYend){
      set(iXbegin,iYbegin,iXend,iYend);
    }
    void set (const int& iXbegin,const int& iYbegin,const int& iXend,const int& iYend){
       begin[0]=iXbegin;
       begin[1]=iYbegin;
       begin[2]=0;
       end[0] = iXend;
       end[1] = iYend;
       end[2] = 1;
    }

    // Begining of the loop over the box, or box origin
    int& getBegin(const int& iD) {return begin[iD];}
    // End of the loop over the box
    int& getEnd  (const int& iD){return end[iD];}
    // Get length of a dimension
    int getL(const int& iD){return end[iD]-begin[iD];}
    // Volume in 2D is surface area of the box
    int getVol(){
        int vol=1;
        for (int iD=0;iD<lattice::nD;++iD){
            vol*=getL(iD);
        }
        return vol;
    }

    void print(){
        for (int iD=0;iD<lattice::nD;++iD){
            cout<<"Dimension="<<iD<<" Begin ="<<begin[iD]<<" End ="<<end[iD]<<" Length="<<getL(iD)<<endl;
        }
        cout<<" Volume="<<getVol()<<endl;
    }
};
