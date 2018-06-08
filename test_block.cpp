#include "block.hh"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
    intND dim={16,16};
    boolND periodic={false,true};

    StructuredDecomposition  structuredDecomposition(dim,periodic);

    Block block(structuredDecomposition);
// Uncomment cout one by one
//    cout<<"~~~~~~~~~~~~~~Block specs for rank = "<<mpiTools.getRank()<<"~~~~~~~~~~~"<<endl<<block;

// if (mpiTools.getRank()==0) std::cout << "boundaryLimit = " << block.getBoundaryLimit()<<'\n';

//if (mpiTools.getRank()==0) std::cout << "ghostLimit = " << block.getGhostLimit()<<'\n';


//if (mpiTools.getRank()==0) std::cout << "sendLimit = " << block.getMessage().getSendLimit()<<'\n';


//if (mpiTools.getRank()==4) std::cout << "recvLimit = " << block.getMessage().getRecvLimit()<<'\n';

//    cout<<"~~~~~~~~~~~~~~Block Neighbors for rank = "<<mpiTools.getRank()<<"~~~~~~~~~~~"<<endl<<block.getMessage().getNeighbor();

if (mpiTools.getRank()==0){
    for (int iQ=0;iQ<lattice::nQ;iQ++){
        vector<int> v1 = block.getMessage().getIOutHalfQs()[iQ];
        vector<int> v2 = block.getMessage().getINotOutHalfQs()[iQ];
        std::cout <<"boundary = "<<iQ<<endl;
        for (auto i=v1.begin();i!=v1.end();i++){
            std::cout<<"  iOutHalfQs = " << *i <<'\n';

        }
    }
}
    MPI_Finalize();
    return 0;
};
