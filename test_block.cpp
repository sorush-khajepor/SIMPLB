#include "block.hh"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
    intND dim={16,16};
    boolND periodic={false,true};

    StructuredDecomposition  structuredDecomposition(dim,periodic);

    Block block(structuredDecomposition);

    cout<<"~~~~~~~~~~~~~~Block specs for rank = "<<mpiTools.getRank()<<"~~~~~~~~~~~"<<endl<<block;


    MPI_Finalize();
    return 0;
};
