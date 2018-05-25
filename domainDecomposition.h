#include <iostream>
#include <math.h>
#include "mpiTools.h"
#include "lattice.h"

class DomainDecomposition{


};

class StructuredDecomposition :public DomainDecomposition {

    protected:
    // Whole domain dimensions
    int dim[lattice::nD];
    // Whole domain volume (number of all nodes before considering ghosts)
    int vol;
    // Number of Blocks in the wholde domain  along each (X-Y-Z) direction
    int nBlock[lattice::nD];
    // Shows the periodicity of domain along cartesian(X-Y-Z) axes
    bool periodic[lattice::nD];

    public:

    StructuredDecomposition(const int dim_[], const bool periodic_[]){

       // Domain initialization
       vol=1;
       for (int iD=0;iD<lattice::nD;++iD){
          dim[iD]=dim_[iD];
          periodic[iD]=periodic_[iD];
	      vol*=dim_[iD];
       }

       // Set number of blocks in the domain. dimXYZ is an Estimation of Block dimension in X/Y/Z-direction.
        int dimXYZ = pow((double)getVol()/mpiTools.getSize(),1.0/lattice::nD);
        for (int iD=0;iD<lattice::nD;++iD){
            nBlock[iD] = dim[iD]/dimXYZ;
        }
    }

    const int& getNBlock(const int& iD) const{
        return nBlock[iD];
    }


    virtual void computeBlockGeometry(int blockOrigin[],int blockDim[],int blockCartIndex[],int iBlockNeighbor[]){
        // Note, here number domain nodes/number of cpus should have squared or
        // cubic root.
        computeBlockCartIndex(mpiTools.getRank(),blockCartIndex);

        for (int iD=0;iD<lattice::nD;++iD){
            int remainder = dim[iD]%nBlock[iD];
            // Dimension of the block along XYZ axes including 2 ghost layers
            blockDim[iD]= (dim[iD]+nBlock[iD]-blockCartIndex[iD] -1)/nBlock[iD]+2;
            // Origin of the block starts from the ghost layer.
            blockOrigin[iD] = dim[iD]/nBlock[iD]*blockCartIndex[iD]+std::min(blockCartIndex[iD],remainder)-1;
        }

        // Setting block neighbors
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            iBlockNeighbor[iQ]=getNeighbor(iQ);
        }
    
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
        return sum;
    }

    void getPeriodicNeighbor (const int iXYZ[], const int& iQ, int iXYZ_neighbor[]) {
        for (int iD = 0; iD < lattice::nD; iD++) {

            iXYZ_neighbor[iD] = (iXYZ[iD] + lattice::Qvector[iQ][iD]+nBlock[iD])%nBlock[iD];
        }
    }

    int getNeighbor (const int& iQ) {
        int blockCartIndex[lattice::nD];
        int neighborCartIndex[lattice::nD];
        computeBlockCartIndex(mpiTools.getRank(),blockCartIndex);
        bool is_null = false;
        for (int iD = 0; iD < lattice::nD; iD++) {
            if (periodic[iD]){
                neighborCartIndex[iD] = (blockCartIndex[iD] + lattice::Qvector[iQ][iD]+nBlock[iD])%nBlock[iD];
            }else{

                neighborCartIndex[iD] = blockCartIndex[iD] + lattice::Qvector[iQ][iD];
            }
            if (neighborCartIndex[iD]<0 or neighborCartIndex[iD]>nBlock[iD]-1){
                is_null = true;
            }
        }
        if (is_null){
            return MPI_PROC_NULL;
        }else{
            return getShortIndex(neighborCartIndex);
        }
    }

    void computeBlockCartIndex (const int& i, int iXYZ[]) {
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
