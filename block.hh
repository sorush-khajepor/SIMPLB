#include "block.h"
#include "message.h"

// Block construction
Block::Block(const StructuredDecomposition& sd){

    intNQ BlockNeighborIndex;

    // Initialization of geometry
    sd.computeBlockGeometry(origin,dim,cartIndex,BlockNeighborIndex);
    vol=dim.computeVol();

    // Memory allocation for nodes
    nodes = new Node[vol];

    // Set loop limits for actual and ghost  boundaries
    setBoundaryLimit();
    setGhostLimit();

    //TODO Set message
    message = new Message (boundaryLimit,ghostLimit,BlockNeighborIndex);
}
