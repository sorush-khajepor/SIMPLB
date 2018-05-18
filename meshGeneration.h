#include <iostream>
#include "block.h"
#include "math.h"
#include "mpiTools.h"

class MeshGeneration{

    virtual void generate()=0;

};

class StructuredMeshGeneration :public MeshGeneration {

    // Whole domain dimensions
    Box dim;

    bool periodic[lattice::nD];

    public:

    virtual void generate(){

    }

    virtual void decompose() {
        // Number of Block nodes in X-direction .There are 2 ghost layers along X-axis
        int nBlockNodeX = sqrt(dim.getVol()/mpiTools.getSize())+2;
        // Number of Blox nodes in Y-direction is equal to X direction. Blocks are boxes.
        int nBlockNodeY = nBlockNodeX;

        int nBlockX = dim.getL(0)/(nBlockNodeX-2);
        int nBlockY = dim.getL(1)/(nBlockNodeY-2);

        // TODO Change block input to Box input
        Block block(nBlockNodeX,nBlockNodeY);


    }

    int getShortIndex (const int& iX, const int& iY) {return iX*dim.getL(1)+iY;}

    void getPeriodicNeighbor (const int& iX, const int& iY, const int& iQ, int& iX_neighbor, int& iY_neighbor) {

        iX_neighbor = (iX + lattice::Qvector[iQ][0]+dim.getL(0))%dim.getL(0);
        iY_neighbor = (iY + lattice::Qvector[iQ][1]+dim.getL(1))%dim.getL(1);
    }


};
