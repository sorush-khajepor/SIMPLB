#include "lattice.h"
#include "mpiTools.h"
//#include "container.h"
#include "block.h"

class Message{
    
    // Neighbor blocks (or MPI processes) in the directions of Qvectors in the MPI world.
    // Note, each Block is assigned to an MPI process.
    arrayNQ<int> neighbor;
    
    // Boundary send buffer, including nQ boundaries. Each is a vector of distros.
    arrayNQ<std::vector<arrayNQ<double> > > boundarySendBuffer;

    // Boundary receive buffer, exactly the same length as send buffer.
    arrayNQ<std::vector<arrayNQ<double> > > boundaryRecvBuffer;

public:

    // Construction
    Message(Block& block, const arrayNQ<int>& neighbor_){

        // set neighbors
        neighbor=neighbor_;

        // boundaries start from iQ = 1. Because borderlimit[iQ=0] gives whole domain.
        for (int iQ=1;iQ<lattice::nQ;++iQ){

            int length = block.getBoundaryLimit()[iQ].getVol();
            if (neighbor[iQ]==MPI_PROC_NULL){
                length = 0;
            }

            for (int iBuffer=0;iBuffer<length;++iBuffer){
                arrayNQ<double> f;
                boundarySendBuffer[iQ].push_back(f);
                boundaryRecvBuffer[iQ].push_back(f);
            }
        }
    }
    
    // Gives the neighbor block (or MPI process) in the lattice direction of iQ.
    const arrayNQ<int>& getNeighbor() const {return neighbor;}

    // Prints class members
    void print() {
        neighbor.print();
    }

    void send(Block& block){

        // Copy domain boundaries to send buffer
        // Boundary Loop
        for (int iQ=1;iQ<lattice::nQ;++iQ){

            if (neighbor[iQ]==MPI_PROC_NULL){continue;}

            int iBuffer =0;
            LoopLimit bl = block.getBoundaryLimit()[iQ];

            for (int iX=bl.getBegin(0);iX<bl.getEnd(0);++iX){
                for (int iY=bl.getBegin(1);iY<bl.getEnd(1);++iY){

                    boundarySendBuffer[iQ][iBuffer] = block(iX,iY);
                    iBuffer++;
                }
            }
        }
    }

};

