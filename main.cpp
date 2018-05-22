

int main(){

// Domain Size
int dim[1] = {10,10};

// Domain periodic vector
bool periodic[1]={true};

// Square domain docomposition
StructuredDecomposition(dim,periodic);

Block(StructuredDecomposition);

// Define bulk collision 
// Define boundary collision
// block.setCollision(collision,box)
// Simulation run

    return 0;
}

