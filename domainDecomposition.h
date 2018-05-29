#include <iostream>
#include <math.h>
#include "mpiTools.h"
#include "lattice.h"

class DomainDecomposition{


};

class StructuredDecomposition :public DomainDecomposition {

    protected:
    // Raw whole domain dimensions (before considering ghosts)
    intND dim;
    // Number of Blocks in the wholde domain  along each (X-Y-Z) direction
    intND nBlock;
    // Shows the periodicity of domain along cartesian(X-Y-Z) axes
    boolND periodic;

    public:

    StructuredDecomposition(const intND dim_, const boolND periodic_){

       // Domain initialization
          dim=dim_;
          periodic=periodic_;

       // Set number of blocks in the domain. dimXYZ is an Estimation of Block dimension in X,Y,Z-direction.
        int dimXYZ = pow((double)dim.computeVol()/mpiTools.getSize(),1.0/lattice::nD);
        nBlock = dim/dimXYZ;
    }

    const int& getNBlock(const int& iD) const{
        return nBlock[iD];
    }

    const intND& getNBlock() const{
        return nBlock;
    }
    const int& getDim(const int& iD) const{
            return dim[iD];
        }

    const intND& getDim() const{
        return dim;
    }

    virtual void computeBlockGeometry(intND blockOrigin,intND blockDim,intND blockCartIndex,intNQ BlockNeighborIndex){

        blockCartIndex=computeBlockCartIndex(mpiTools.getRank());

        for (int iD=0;iD<lattice::nD;++iD){
            int remainder = dim[iD]%nBlock[iD];
            // Dimension of the block along XYZ axes including 2 ghost layers
            blockDim[iD]= (dim[iD]+nBlock[iD]-blockCartIndex[iD] -1)/nBlock[iD]+2;
            // Origin of the block starts from the ghost layer.
            blockOrigin[iD] = dim[iD]/nBlock[iD]*blockCartIndex[iD]+std::min(blockCartIndex[iD],remainder)-1;
        }

        // Setting block neighbors
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            BlockNeighborIndex[iQ]=computeNeighborIndex(iQ);
        }
    
    }


    intND computePeriodicNeighbor (const intND blockCartIndex, const int& iQ ) {
        intND neighborCartIndex;
        for (int iD = 0; iD < lattice::nD; iD++) {
            neighborCartIndex[iD] = (blockCartIndex[iD] + lattice::Qvector[iQ][iD]+nBlock[iD])%nBlock[iD];
        }

         return neighborCartIndex;
    }

    int computeNeighborIndex (const int& iQ) {
        intND neighborCartIndex;
        intND blockCartIndex=computeBlockCartIndex(mpiTools.getRank());
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
            return computeBlockSingleIndex(neighborCartIndex);
        }
    }

    int computeBlockSingleIndex (const intND BlockCartIndex) {
            int sum=0;
            for (int j = 0; j < lattice::nD; j++) {
                int n = BlockCartIndex[j];
                for (int k = j+1; k < lattice::nD; k++) {
                    n*= nBlock[k];
                }
                sum += n;
            }
            return sum;
        }

    // compute Cartesian index of a block
    intND computeBlockCartIndex (const int& blockSingleIndex) {
        intND iXYZ;
        int R = blockSingleIndex;
        for (int j = 0; j < lattice::nD; j++) {
            int n =1;
            for (int k = j+1; k < lattice::nD; k++) {
                n*=nBlock[k];
            }
            iXYZ[j]= R/n;
            R=R%n;
        }
        return iXYZ;
    }

};
