#include "block.h"
#include "domainDecomposition.h"

int main(){

// Domain Size
int dim[2] = {10,10};

// Domain periodic vector
bool periodic[2]={true};

// Square domain docomposition
StructuredDecomposition(dim,periodic);

//Block block(StructuredDecomposition);

// Define bulk collision 
// Define boundary collision
// block.setCollision(collision,box)
// Simulation run

    return 0;
}

