#include "domainDecomposition.h"
using namespace std;
int main() {
    

   intND dim={16,16};
   boolND periodic={true,true,true};

   StructuredDecomposition  s2d(dim,periodic);

//   cout<<"\n nBlock,Dim, rank="<<mpiTools.getRank()<<s2d.getNBlock()<<s2d.getDim()<<endl;

   // rank should be equal to SingleIndex
   intND bci=s2d.computeBlockCartIndex(mpiTools.getRank());
//   cout<<"\n CartIndex,SingleIndex, rank="<<mpiTools.getRank()<<bci<<"single Index="<<s2d.computeBlockSingleIndex(bci)<<endl;

//   cout<<"\n Neighbor Index, rank="<<mpiTools.getRank()<<s2d.computeNeighborIndex();

   intND blockOrigin;
   intND blockDim;
   intND blockCartIndex;
   intNQ BlockNeighborIndex;
   s2d.computeBlockGeometry(blockOrigin,blockDim,blockCartIndex,BlockNeighborIndex);
   cout<<"Origin, Dim, iBlockCart, Neighbors rank = "<<mpiTools.getRank()<<blockOrigin<<blockDim<<blockCartIndex<<BlockNeighborIndex<<endl;
   MPI_Finalize();



    return 0;
}
