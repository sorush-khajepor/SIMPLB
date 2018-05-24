#include "lattice.h"
#include "mpiTools.h"

class Message{
    
    // Neighbor blocks (or MPI processes) in the directions of Qvectors in the MPI world.
    // Note, each Block is assigned to an MPI process.
    int neighbor[lattice::nQ];
    
public:
    Message(const int neighbor_[]){
        for (int iQ=0;iQ<lattice::nQ;iQ++){
            neighbor[iQ]=neighbor_[iQ];
        }
    }
    
    // Gives the neighbor block (or MPI process) in the lattice direction of iQ.
    int& getNeighbor (const int& iQ) {return neighbor[iQ];}





};

