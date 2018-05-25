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

    // Limits of loops for covering block boundaries (ignoring ghosts and non-interacting neighbors)
    arrayNQ<LoopLimit> sendLimit;

    // Limits of loops for covering block boundaries (including ghosts but ignoring non-interacting neighbors)
    arrayNQ<LoopLimit> recvLimit;

public:

    // Construction
    Message(Block& block, const arrayNQ<int>& neighbor_){

        // set neighbors
        neighbor=neighbor_;

        // set limits of send & receive buffer
        sendLimit = block.getBoundaryLimit();
        recvLimit = block.getGhostLimit();
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            if (neighbor[iQ]==MPI_PROC_NULL){
                sendLimit[iQ].set(0,0,0,0);
                recvLimit[iQ].set(0,0,0,0);
            }
        }

        // set send&receive buffer
        // boundaries start from iQ = 1. Because borderlimit[iQ=0] gives whole domain.
        for (int iQ=1;iQ<lattice::nQ;++iQ){

            int length = block.getBoundaryLimit()[iQ].getVol();
            if (neighbor[iQ]==MPI_PROC_NULL){
                length = 0;
            }

            for (int iBuffer=0;iBuffer<length;++iBuffer){
                arrayNQ<double> f;
                boundarySendBuffer[iQ].push_back(f);
                boundaryRecvBuffer[iQ].push_back(f) ;
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
        // Loop over all boundaries
        for (int iQ=1;iQ<lattice::nQ;++iQ){

            // No neighbor, no copy!
            if (neighbor[iQ]==MPI_PROC_NULL){continue;}

            int iBuffer =0;
            LoopLimit bl = block.getBoundaryLimit()[iQ];

            // Sweeping inside a boundary
            for (int iX=bl.getBegin(0);iX<bl.getEnd(0);++iX){
                for (int iY=bl.getBegin(1);iY<bl.getEnd(1);++iY){

                    boundarySendBuffer[iQ][iBuffer] = block(iX,iY);
                    iBuffer++;
                }
            }
        }


        // MPI Send
        arrayNQ<MPI_Request> req;
        // Loop over different boundaries
        for (int iQ=1;iQ<lattice::nQ;++iQ){
            int length = block.getBoundaryLimit()[iQ].getVol();
            if (neighbor[iQ]==MPI_PROC_NULL){continue;}
            MPI_Isend(&boundarySendBuffer[iQ][0], length*lattice::nQ, MPI_DOUBLE,neighbor[iQ], iQ, MPI_COMM_WORLD,&req[iQ]);
        }
    }


    void receive(Block& block){

        // Receive buffer
        arrayNQ<MPI_Status> status;
        for (int iQ=1;iQ<lattice::nQ;++iQ){
            int iOp = lattice::iOpposite[iQ];
            int length = block.getBoundaryLimit()[iQ].getVol();
            if (neighbor[iQ]==MPI_PROC_NULL){continue;}
            MPI_Recv(&boundaryRecvBuffer[iQ][0], length*lattice::nQ, MPI_DOUBLE,neighbor[iQ], iOp, MPI_COMM_WORLD,&status[iQ]);
        }


        // Copying receive buffer into block ghost nodes
        for (int iQ=1;iQ<lattice::nQ;++iQ){
            if (neighbor[iQ]==MPI_PROC_NULL){continue;}
            int iBuffer =0;
            LoopLimit gl = block.getGhostLimit()[iQ];
            for (int iX=gl.getBegin(0);iX<gl.getEnd(0);++iX){
                for (int iY=gl.getBegin(1);iY<gl.getEnd(1);++iY){

                    boundaryRecvBuffer[iQ][iBuffer].copyTo(block(iX,iY));
                    iBuffer++;
                }
            }
        }
    }

};

