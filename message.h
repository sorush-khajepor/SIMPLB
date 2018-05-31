#include "lattice.h"
#include "mpiTools.h"
#include "block.h"
#include <vector>


class Message{

    // Neighbor blocks (or MPI processes) in the directions of Qvectors in the MPI world.
    // Note, each Block is assigned to an MPI process.
    intNQ neighbor;

    // Boundary send buffer, including nQ boundaries. Each is a vector of distros.
    SArrayBase<std::vector<doubleNQ>,lattice::nQ> boundarySendBuffer;

    // Boundary receive buffer, exactly the same length as send buffer.
    SArrayBase<std::vector<doubleNQ>,lattice::nQ> boundaryRecvBuffer;

    // Limits of loops for send buffer (ignoring ghosts and non-interacting neighbors)
    LoopLimitNQ sendLimit;

    // Limits of loops for receive buffer (including ghosts but ignoring non-interacting neighbors)
    LoopLimitNQ recvLimit;

public:

    LoopLimitNQ getSendLimit() {return sendLimit;}
    LoopLimitNQ getRecvLimit() {return recvLimit;}
    // Construction
    Message(LoopLimitNQ& blockBoundaryLimit,LoopLimitNQ& blockGhostLimit, const intNQ& neighbor_){

        // set neighbors
        neighbor=neighbor_;

        // set limits of send & receive buffer
        sendLimit = blockBoundaryLimit;
        recvLimit = blockGhostLimit;
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            if (neighbor[iQ]==MPI_PROC_NULL){
                sendLimit[iQ]={{0,0},{0,0}};
                recvLimit[iQ]={{0,0},{0,0}};
            }
        }

        // set send&receive buffer
        // Boundaries start from iQ = 1. because borderlimit[iQ=0] gives whole domain.
        // Note, computeVol() for sendLimit is equal to recvLimit. But they have different beginning and end.
        for (int iQ=1;iQ<lattice::nQ;++iQ){

            int length = sendLimit[iQ].computeVol();
            for (int iBuffer=0;iBuffer<length;++iBuffer){
                doubleNQ f;
                boundarySendBuffer[iQ].push_back(f);
                boundaryRecvBuffer[iQ].push_back(f) ;
            }
        }
    }

    // Gives the neighbor block (or MPI process) in the lattice direction of iQ.
    const intNQ& getNeighbor() const {return neighbor;}


    void send(Block& block){

        // Copy domain boundaries to send buffer
        // Loop over all boundaries
        for (int iQ=1;iQ<lattice::nQ;++iQ){

            int iBuffer =0;
            LoopLimit sl = sendLimit[iQ];

            // Sweeping inside a boundary
            for (int iX=sl.getBegin(0);iX<sl.getEnd(0);++iX){
                for (int iY=sl.getBegin(1);iY<sl.getEnd(1);++iY){

                    boundarySendBuffer[iQ][iBuffer] = block(iX,iY).getF();
                    iBuffer++;
                }
            }
        }


        // MPI Send
        SArrayBase<MPI_Request,lattice::nQ> req;
        // Loop over different boundaries
        for (int iQ=1;iQ<lattice::nQ;++iQ){
            int length = sendLimit[iQ].computeVol();
            MPI_Isend(&boundarySendBuffer[iQ][0], length*lattice::nQ, MPI_DOUBLE,neighbor[iQ], iQ, MPI_COMM_WORLD,&req[iQ]);
        }
    }


    void receive(Block& block){

        // MPI Receive buffer
        SArrayBase<MPI_Status,lattice::nQ> status;
        for (int iQ=1;iQ<lattice::nQ;++iQ){
            int iOp = lattice::iOpposite[iQ];
            int length = recvLimit[iQ].computeVol();
            MPI_Recv(&boundaryRecvBuffer[iQ][0], length*lattice::nQ, MPI_DOUBLE,neighbor[iQ], iOp, MPI_COMM_WORLD,&status[iQ]);
        }


        // Copying receive buffer into block ghost nodes
        for (int iQ=1;iQ<lattice::nQ;++iQ){
            int iBuffer =0;
            LoopLimit rl = recvLimit[iQ];
            for (int iX=rl.getBegin(0);iX<rl.getEnd(0);++iX){
                for (int iY=rl.getBegin(1);iY<rl.getEnd(1);++iY){

                    block(iX,iY).getF()=boundaryRecvBuffer[iQ][iBuffer];
                    iBuffer++;
                }
            }
        }
    }

};
