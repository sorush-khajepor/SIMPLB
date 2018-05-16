#include <iostream>
#include "block.h"
#include "math.h"
#include "mpiTools.h"


using namespace std;

struct BoxLimit{
	int iXbegin, iXend;
	int iYbegin, iYend;
};

void getLongIndex (const int& i, const int&nY, int& iX, int& iY)  {iX = i/nY; iY=i%nY;}	


int getShortIndex (const int& iX, const int& iY, const int& nY) {return iX*nY+iY;}
void getPeriodicNeighbor (const int& iX, const int& iY, const int& iQ,const int&nX, const int& nY, int& iX_neighbor, int& iY_neighbor) {

	iX_neighbor = (iX + lattice::Qvector[iQ][0]+nX)%nX;
        iY_neighbor = (iY + lattice::Qvector[iQ][1]+nY)%nY;

}

int main(){



int nDomainNodeX=10;
int nDomainNodeY=10;
int nMortarLayer = 2;
int nDomainNode = nDomainNodeX*nDomainNodeY;
int nBlockNodeX = sqrt(nDomainNode/mpiTools.getSize())+nMortarLayer;
int nBlockNodeY = nBlockNodeX;
int nBlockX = nDomainNodeX/(nBlockNodeX-nMortarLayer);
int nBlockY = nDomainNodeY/(nBlockNodeY-nMortarLayer);
Block block(nBlockNodeX,nBlockNodeY);

// filling a node on top of block0 with 1.0
if (mpiTools.getRank()==0){
    for (int iQ=0;iQ<lattice::nQ;++iQ){
        block(5,5)[5] = 5.0;//(double) iQ+0.00001;
    }
}
// rank is the address of each block

//cout<<"nBlockX,Y "<<nBlockX<<" "<<nBlockY<<endl;
//cout<<"nBlockNodeX,Y "<<nBlockNodeX<<" "<<nBlockNodeY<<endl;

// setting block neighbors
int iX,iY,iXnei,iYnei;
getLongIndex(mpiTools.getRank(),nBlockY,iX,iY);
for (int iQ=0;iQ<lattice::nQ;++iQ){
	getPeriodicNeighbor(iX,iY,iQ,nBlockX,nBlockY,iXnei,iYnei);
	block.getMPIneighbor(iQ)= getShortIndex(iXnei,iYnei,nBlockY);
//	cout<<mpiTools.getRank()<<" iX="<<iX<<" iY="<<iY<<" iQ= "<<iQ
//	 <<" nei_rank="<<block.getMPIneighbor(iQ)<<" iXnei="<<iXnei<<" iYnei="<<iYnei<<endl;
}

// setting border limits(not mortar layer) for D2Q9 - check for loop limits of 0 if it runs!
BoxLimit blockLimit[lattice::nQ];
{
int NNX=block.getNX()-1;
int NNY=block.getNY()-1;
int BBegin = 1;
for (int iQ=0;iQ<lattice::nQ;++iQ){
    if (lattice::Qvector[iQ][0]==1){
        blockLimit[iQ].iXbegin = NNX-1;
        blockLimit[iQ].iXend = NNX;
    }else if(lattice::Qvector[iQ][0]==0){
        blockLimit[iQ].iXbegin=BBegin;
        blockLimit[iQ].iXend=NNX;
    }else if(lattice::Qvector[iQ][0]==-1){
        blockLimit[iQ].iXbegin=BBegin;
        blockLimit[iQ].iXend=BBegin+1;
    }

    if (lattice::Qvector[iQ][1]==1){
        blockLimit[iQ].iYbegin = NNY-1;
        blockLimit[iQ].iYend = NNY;
    }else if(lattice::Qvector[iQ][1]==0){
        blockLimit[iQ].iYbegin=BBegin;
        blockLimit[iQ].iYend=NNY;
    }else if(lattice::Qvector[iQ][1]==-1){
        blockLimit[iQ].iYbegin=BBegin;
        blockLimit[iQ].iYend=BBegin+1;
    }

    if (mpiTools.getRank()==0){

    //	cout<<blockLimit[iQ].iXbegin<<"  "<<blockLimit[iQ].iXend<<"  "<<blockLimit[iQ].iYbegin<<"  "<<blockLimit[iQ].iYend<<endl;

    }
}
}
// setting Mortar Layer limits for D2Q9 - check for loop limits of 0 if it runs!
BoxLimit mortarLimit[lattice::nQ];
int NNX=block.getNX();
int NNY=block.getNY();
int BBegin = 0;
for (int iQ=0;iQ<lattice::nQ;++iQ){
    if (lattice::Qvector[iQ][0]==1){
        mortarLimit[iQ].iXbegin = NNX-1;
        mortarLimit[iQ].iXend = NNX;
    }else if(lattice::Qvector[iQ][0]==0){
        mortarLimit[iQ].iXbegin=BBegin+1;
        mortarLimit[iQ].iXend=NNX-1;
    }else if(lattice::Qvector[iQ][0]==-1){
        mortarLimit[iQ].iXbegin=BBegin;
        mortarLimit[iQ].iXend=BBegin+1;
    }

    if (lattice::Qvector[iQ][1]==1){
        mortarLimit[iQ].iYbegin = NNY-1;
        mortarLimit[iQ].iYend = NNY;
    }else if(lattice::Qvector[iQ][1]==0){
        mortarLimit[iQ].iYbegin=BBegin+1;
        mortarLimit[iQ].iYend=NNY-1;
    }else if(lattice::Qvector[iQ][1]==-1){
        mortarLimit[iQ].iYbegin=BBegin;
        mortarLimit[iQ].iYend=BBegin+1;
    }

    if (mpiTools.getRank()==0){

    //	cout<<mortarLimit[iQ].iXbegin<<"  "<<mortarLimit[iQ].iXend<<"  "<<mortarLimit[iQ].iYbegin<<"  "<<mortarLimit[iQ].iYend<<endl;

    }
}



// creating node vectors of boundaries
//std::vector<Node> border[lattice::nQ];
/*
MPI_Datatype border[lattice::nQ];
int blocklength = 1;
int stride;

int iQ = 1;
int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
if (iQ==2 or iQ==4){
    stride = block.getNY();
} else {
    stride = 1;
}

MPI_Type_vector(length,blocklength,stride,Node,&border[iQ]);
*/
// initialization of boundary buffer
class Distro{
        double f[lattice::nQ]={0.};
public:
        double& getF(const int& iQ){
            return f[iQ];
        }
        Distro& operator= (const Distro& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    f[iQ] = rhs.f[iQ];
                }
            }
        Distro& operator= (const Node& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    f[iQ] = rhs[iQ];
                }
            }
};

std::vector<Distro> boundarySendBuffer[lattice::nQ];
std::vector<Distro> boundaryRecvBuffer[lattice::nQ];

// boundaries start from iQ = 1. Because borderlimit[iQ=0] gives whole domain.
for (int iQ=1;iQ<lattice::nQ;++iQ){
    int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
    for (int iBuffer=0;iBuffer<length;++iBuffer){
        Distro distro;
        boundarySendBuffer[iQ].push_back(distro);
        boundaryRecvBuffer[iQ].push_back(distro);
    }
}

//    for (int iX=blockLimit[iBoundary].iXbegin;iX<blockLimit[iBoundary].iXend;++iX){
//        for (int iY=blockLimit[iBoundary].iYbegin;iY<blockLimit[iBoundary].iYend;++iY){
//    int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
//    for (int iBuffer=0;iBuffer<length;++iBuffer){



// Send Boundary Buffers
/*
MPI_Request reqs,reqr;
MPI_Status status;
//for (int iQ=1;iQ<lattice::nQ;++iQ)
if (mpiTools.getRank()==0)
{
    int iQ = 2;
    int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
// printing
    for(int ib=0;ib<length;++ib)
    {
        for (int i=0;i<lattice::nQ;++i)
        cout<<"Rank()="<<mpiTools.getRank()<<"  node ID="<<ib<<"  dir="<<i<<"  f= "<<boundarySendBuffer[iQ][ib].getF(i)<<endl;
    }
    cout<<"-----------------------"<<endl;
// printing finished
    MPI_Isend(&boundarySendBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,block.getMPIneighbor(iQ), iQ, MPI_COMM_WORLD,&reqs);
}
//for (int iQ=1;iQ<lattice::nQ;++iQ)
if(mpiTools.getRank()==1)
{
    int iQ = 4;
    int iOp = lattice::iOpposite[iQ];
    int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
    MPI_Recv(&boundaryRecvBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,block.getMPIneighbor(iOp), iOp, MPI_COMM_WORLD,&status);

// printing
    for(int ib=0;ib<length;++ib)
    {
        for (int i=0;i<lattice::nQ;++i)
        cout<<" AfterRank"<<mpiTools.getRank()<<"  node ID="<<ib<<"  dir="<<i<<"  f= "<<boundaryRecvBuffer[iQ][ib].getF(i)<<endl;
    }
    cout<<"-----------------------"<<endl;
// printing finished

}
*/

for (int t=0;t<10;++t){
// Copy domain boundaries to send buffer
for (int iQ=1;iQ<lattice::nQ;++iQ){
    int iBuffer =0;
    for (int iX=blockLimit[iQ].iXbegin;iX<blockLimit[iQ].iXend;++iX){
        for (int iY=blockLimit[iQ].iYbegin;iY<blockLimit[iQ].iYend;++iY){

            boundarySendBuffer[iQ][iBuffer] = block(iX,iY);
            iBuffer++;
        }
    }
}

// Send & Recv
MPI_Request reqs,reqr;
MPI_Status status;
for (int iQ=1;iQ<lattice::nQ;++iQ)
{
    int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
    MPI_Isend(&boundarySendBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,block.getMPIneighbor(iQ), iQ, MPI_COMM_WORLD,&reqs);
}
for (int iQ=1;iQ<lattice::nQ;++iQ)
{
    int iOp = lattice::iOpposite[iQ];
    int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
    MPI_Recv(&boundaryRecvBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,block.getMPIneighbor(iQ), iOp, MPI_COMM_WORLD,&status);
}

// Copying receive buffer into Domain mortar nodes
for (int iQ=1;iQ<lattice::nQ;++iQ){
    int iBuffer =0;
    for (int iX=mortarLimit[iQ].iXbegin;iX<mortarLimit[iQ].iXend;++iX){
        for (int iY=mortarLimit[iQ].iYbegin;iY<mortarLimit[iQ].iYend;++iY){
            // TODO :: overload this loop
            for (int iQQ=0;iQQ<lattice::nQ;++iQQ){
                block(iX,iY)[iQQ] = boundaryRecvBuffer[iQ][iBuffer].getF(iQQ);
            }

            iBuffer++;
        }
    }
}

block.stream();
//block.revStream(1,block.getNX(),1,block.getNY());
MPI_Barrier(MPI_COMM_WORLD);
// Printing
for (int iX=0;iX<block.getNX();++iX){
    for (int iY=0;iY<block.getNY();++iY){
        for (int iQ=0;iQ<lattice::nQ;++iQ)
        {
            double f = block(iX,iY)[iQ];
            if (f>0.){
                cout<<" t="<<t<<" Rank="<<mpiTools.getRank()<<"  node iX="<<iX<<" node iY="<<iY<<"  dir="<<iQ<<"  f= "<<f<<endl;
            }
        }
    }
}


// printing
for (int iQ=1;iQ<lattice::nQ;++iQ)
{
    int length = (blockLimit[iQ].iYend - blockLimit[iQ].iYbegin)*(blockLimit[iQ].iXend - blockLimit[iQ].iXbegin);
    for(int ib=0;ib<length;++ib)
    {
        for (int i=0;i<lattice::nQ;++i)
        {
            double f = boundaryRecvBuffer[iQ][ib].getF(i);
            if (f>0.){
              //  cout<<" rank="<<mpiTools.getRank()<<"  BC dir="<<iQ<<"  node ID="<<ib<<"  dir="<<i<<"  f= "<<f<<endl;
            }
        }
    }
}

// printing finished


} //timestep loop

MPI_Finalize();

return 0;
}


// int loadBalancer (int N,int &begin,int &end){

