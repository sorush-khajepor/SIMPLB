#include <iostream>
#include "block.h"
#include <math.h>
#include "mpiTools.h"

class MeshGeneration{

    virtual void generate()=0;

};

class StructuredMeshGeneration :public MeshGeneration {

    // Whole domain dimensions
    int dim[lattice::nD]={0};
    int nBlock[lattice::nD]={0};
    int vol=0;
    bool periodic[lattice::nD]={true};

    public:

    virtual void generate(){

        int nBlockNode[lattice::nD]; 
        // Number of Block nodes in X-direction .There are 2 ghost layers along X-axis
        nBlockNode[0] = pow((double)getVol()/mpiTools.getSize(),1.0/lattice::nD)+2;
        // Number of Blox nodes in Y-direction is equal to X direction. Blocks are boxes.
        for (int iD=1;iD<lattice::nD;++iD){
            nBlockNode[iD] = nBlockNode[0];
        }

        for (int iD=0;iD<lattice::nD;++iD){
            nBlock[iD] = dim[iD]/(nBlockNode[iD]-2);
        }
    
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

            iXYZ_neighbor[iD] = (iXYZ[iD] + lattice::Qvector[iQ][iD]+nBlock[iD])%nBlock[iD];
        }
    }

    void getLongIndex (const int& i, int& iXYZ[]) 
    {

        int R = i;
        for (int j = 0; j < lattice::nD; j++) {
            int n =1;
            for (int k = j+1; k < lattice::nD; k++) {
                n*=nBlock[k];
            }
                iXYZ[j]= R/n;
                R=R%n;
        }

    }

};
