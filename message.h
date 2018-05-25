#include "lattice.h"
#include "mpiTools.h"
#include "container.h"

class Message{
    
    // Neighbor blocks (or MPI processes) in the directions of Qvectors in the MPI world.
    // Note, each Block is assigned to an MPI process.
    arrayNQ<int> neighbor;
    
public:
    Message(const arrayNQ<int> neighbor_){
        neighbor=neighbor_;
    }
    
    // Gives the neighbor block (or MPI process) in the lattice direction of iQ.
    const int& getNeighbor (const int& iQ) const {return neighbor[iQ];}
    const arrayNQ<int>& getNeighbor () const {return neighbor;}

    // Prints class members
    void print() {
        neighbor.print();
    }



};

