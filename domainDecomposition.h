#include <iostream>
//#include "block.h"
#include <math.h>
#include "mpiTools.h"
#include "lattice.h"

class DomainDecomposition{

    virtual void decompose()=0;

};

class StructuredDecomposition :public DomainDecomposition {

    protected:
    // Whole domain dimensions
    int dim[lattice::nD];
    // Whole domain volume (number of all nodes before considering ghosts)
    int vol;
    // Number of Blocks in the wholde domain  along each (X-Y-Z) direction
    int nBlock[lattice::nD];
    // Number of Nodes inside the block of this computer including ghosts
    int nBlockNode[lattice::nD];
    // Shows the periodicity of domain along cartesian(X-Y-Z) axes
    bool periodic[lattice::nD];

    // Cartesian index of the block in domain or MPI world
    int blockCartIndex[lattice::nD];

    public:

    StructuredDecomposition(const int dim_[], const bool periodic_[]){

       // Domain initialization
       vol=1;
       for (int iD=0;iD<lattice::nD;++iD){
          dim[iD]=dim_[iD];
          periodic[iD]=periodic_[iD];
	      vol*=dim_[iD];
       }

       coomputeBlockCartIndex(mpiTools.getRank(),blockCartIndex);

       decompose();

    }

    const int& getNBlock(const int& iD) const{
        return nBlock[iD];
    }


    const int& getNBlockNode(const int& iD) const{
        return nBlockNode[iD];
    }


    virtual void decompose(){
        // Note, here number domain nodes/number of cpus should have squared or
        // cubic root.
        // Estimation of Number of Block nodes in X/Y/Z-direction.
        nNodeXYZ = pow((double)getVol()/mpiTools.getSize(),1.0/lattice::nD);
        // Number of Block nodes along Y&Z axes is equal to X axis.
/*        for (int iD=1;iD<lattice::nD;++iD){
            nBlockNode[iD] = nBlockNode[0];
      }*/

        for (int iD=0;iD<lattice::nD;++iD){
            nBlock[iD] = dim[iD]/nNodeXYZ;
        }


        int remainder[lattice::nD];
        for (int iD=0;iD<lattice::nD;++iD){
            remainder[iD] = dim[iD]%nBlock[iD];
            nBlockNode[iD]= (dim[iD]+nBlock[iD]-
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

    void computeCartIndex (const int& i, int iXYZ[]) {

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
