#include "domainDecomposition.h"
using namespace std;
int main() {
    
// Uncomment cout commands to check each part
   intND dim={16,16};
   boolND periodic={false,true};

   StructuredDecomposition  s2d(dim,periodic);

//   cout<<"\n nBlock,Dim, rank="<<mpiTools.getRank()<<s2d.getNBlock()<<s2d.getDim()<<endl;

   intND bci=s2d.computeBlockCartIndex(mpiTools.getRank());
//   cout<<"\n CartIndex,SingleIndex, rank="<<mpiTools.getRank()<<bci<<"single Index="<<s2d.computeBlockSingleIndex(bci)<<endl;

   cout<<"\n Neighbor Index, rank="<<mpiTools.getRank()<<s2d.computeNeighborIndex();

   intND blockOrigin;
   intND blockDim;
   intND blockCartIndex;
   intNQ BlockNeighborIndex;
   s2d.computeBlockGeometry(blockOrigin,blockDim,blockCartIndex,BlockNeighborIndex);
//   cout<<"Origin, Dim, iBlockCart, Neighbors rank = "<<mpiTools.getRank()<<blockOrigin<<blockDim<<blockCartIndex<<BlockNeighborIndex<<endl;
   MPI_Finalize();



    return 0;
}
/*

MPI n = 9

Domain = 16x16 = (6x5x5) x (6x5x5)

block(with ghosts) = 8x7x7


                     8                7                  7

            +---------------+-------------------+------------------+
            |               |                   |                  |
            |     (0,2)     |       (1,2)       |      (2,2)       |
       7    |      2        |        5          |       8          |
            |               |                   |                  |
            |               |                   |                  |
            |               |                   |                  |
            +------------------------------------------------------+
            |     (0,1)     |       (1,1)       |       (2,1)      |
            |               |                   |                  |
            |      1        |        4          |        7         |
       7    |               |                   |                  |
            |               |                   |                  |
            |               |                   |                  |
            +------------------------------------------------------+
            |               |                   |                  |
            |     (0,0)     |        (1,0)      |       (2,0)      |
            |               |                   |                  |
       8    |      0        |        3          |        6         |
            |               |                   |                  |
            |               |                   | 12  13 14 15  16 | 17*
            |             6*| 7   8   9  10  11 | 12*              |
            +-----------------------------------+------------------+
          0* 1  2 3  4 5  6   7*

Origin ->       (0,0)           (6,0)               (11,0)          *=Ghost node

*/
