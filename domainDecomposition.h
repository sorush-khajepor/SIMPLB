#include <iostream>
#include <math.h>
#include "mpiTools.h"
#include "lattice.h"

class DomainDecomposition{


};

class StructuredDecomposition :public DomainDecomposition {

    protected:
    // Origin is placed on (2,2[,2])
    const intND origin=2;
    // Raw whole domain dimensions/number of nodes along (X-Y-z) before considering ghosts
    intND dim;
    // Number of Blocks in the whole domain along each (X-Y-Z) direction
    intND nBlock;
    // Shows the periodicity of domain along Cartesian (X-Y-Z) axes
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

    virtual void computeBlockGeometry(intND& blockOrigin,intND& blockDim,intND& blockCartIndex,intNQ& BlockNeighborIndex) const {

        blockCartIndex=computeBlockCartIndex(mpiTools.getRank());

        for (int iD=0;iD<lattice::nD;++iD){
            int remainder = dim[iD]%nBlock[iD];
            // Dimension of the block along XYZ axes including 4 ghost layers
            blockDim[iD]= (dim[iD]+nBlock[iD]-blockCartIndex[iD] -1)/nBlock[iD]+4;
            // Origin of the block starts from the outer ghost layer.
            blockOrigin[iD] = dim[iD]/nBlock[iD]*blockCartIndex[iD]+std::min(blockCartIndex[iD],remainder)-2 + origin[iD];
        }


        BlockNeighborIndex=computeNeighborIndex();

    }

    // Finding block neighbors
    intNQ computeNeighborIndex() const {
        intNQ iNei;
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            iNei[iQ]=computeNeighborIndex(iQ);
        }
        return iNei;
    }

    intND computePeriodicNeighbor (const intND blockCartIndex, const int& iQ ) const {
        intND neighborCartIndex;
        for (int iD = 0; iD < lattice::nD; iD++) {
            neighborCartIndex[iD] = (blockCartIndex[iD] + lattice::Qvector[iQ][iD]+nBlock[iD])%nBlock[iD];
        }

         return neighborCartIndex;
    }

    int computeNeighborIndex (const int& iQ) const {
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

    int computeBlockSingleIndex (const intND BlockCartIndex)const {
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

    // Compute Cartesian index of a block
    intND computeBlockCartIndex (const int& blockSingleIndex) const {
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
