#include <iostream>
#include "block.h"
#include "math.h"
#include "mpiTools.h"

class MeshGeneration{

    virtual void generate()=0;

};

class StructuredMeshGeneration :public MeshGeneration {

    // Whole domain dimensions
    int dim[lattice::nD];
    int vol;
    bool periodic[lattice::nD];

    public:

    virtual void generate(){

    }

    virtual void decompose() {
        // Number of Block nodes in X-direction .There are 2 ghost layers along X-axis
        int nBlockNodeX = sqrt(getVol()/mpiTools.getSize())+2;
        // Number of Blox nodes in Y-direction is equal to X direction. Blocks are boxes.
        int nBlockNodeY = nBlockNodeX;

        int nBlockX = dim[0]/(nBlockNodeX-2);
        int nBlockY = dim[1]/(nBlockNodeY-2);

        // TODO Change block input to Box input
        Block block(nBlockNodeX,nBlockNodeY);


    }

    const int& getVol() const {return vol;}

    int getShortIndex (const int iXYZ[]) {
        int sum=0;
        for (int j = 0; j < lattice::nD; j++) {
            int n = iXYZ[j];
            for (int k = j+1; k < lattice::nD; k++) {
                n*= dim[k];
            }
            sum += n;
        }
        return sum;}

    void getPeriodicNeighbor (const int iXYZ[], const int& iQ, int iXYZ_neighbor[]) {
        for (int iD = 0; iD < lattice::nD; iD++) {

            iXYZ_neighbor[iD] = (iXYZ[iD] + lattice::Qvector[iQ][iD]+dim[iD])%dim[iD];
        }
    }


};
