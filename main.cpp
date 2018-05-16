#include <iostream>
#include "brick.h"
#include "math.h"
#include "mpi.h"


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

MPI_Init(NULL,NULL);
int rank;
int size;

MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size); 

int nDomainNodeX=10;
int nDomainNodeY=10;
int nMortarLayer = 2;
int nDomainNode = nDomainNodeX*nDomainNodeY;
int nBrickNodeX = sqrt(nDomainNode/size)+nMortarLayer;
int nBrickNodeY = nBrickNodeX;
int nBrickX = nDomainNodeX/(nBrickNodeX-nMortarLayer);
int nBrickY = nDomainNodeY/(nBrickNodeY-nMortarLayer);
Brick brick(nBrickNodeX,nBrickNodeY);

// filling a node on top of brick0 with 1.0
if (rank==0){
    for (int iQ=0;iQ<lattice::nQ;++iQ){
        brick(5,5)[5] = 5.0;//(double) iQ+0.00001;
    }
}
// rank is the address of each brick

//cout<<"nBrickX,Y "<<nBrickX<<" "<<nBrickY<<endl;
//cout<<"nBrickNodeX,Y "<<nBrickNodeX<<" "<<nBrickNodeY<<endl;

// setting brick neighbors
int iX,iY,iXnei,iYnei;
getLongIndex(rank,nBrickY,iX,iY);
for (int iQ=0;iQ<lattice::nQ;++iQ){
	getPeriodicNeighbor(iX,iY,iQ,nBrickX,nBrickY,iXnei,iYnei);
	brick.getMPIneighbor(iQ)= getShortIndex(iXnei,iYnei,nBrickY);
//	cout<<rank<<" iX="<<iX<<" iY="<<iY<<" iQ= "<<iQ
//	 <<" nei_rank="<<brick.getMPIneighbor(iQ)<<" iXnei="<<iXnei<<" iYnei="<<iYnei<<endl;
}

// setting border limits(not mortar layer) for D2Q9 - check for loop limits of 0 if it runs!
BoxLimit brickLimit[lattice::nQ];
{
int NNX=brick.getNX()-1;
int NNY=brick.getNY()-1;
int BBegin = 1;
for (int iQ=0;iQ<lattice::nQ;++iQ){
    if (lattice::Qvector[iQ][0]==1){
        brickLimit[iQ].iXbegin = NNX-1;
        brickLimit[iQ].iXend = NNX;
    }else if(lattice::Qvector[iQ][0]==0){
        brickLimit[iQ].iXbegin=BBegin;
        brickLimit[iQ].iXend=NNX;
    }else if(lattice::Qvector[iQ][0]==-1){
        brickLimit[iQ].iXbegin=BBegin;
        brickLimit[iQ].iXend=BBegin+1;
    }

    if (lattice::Qvector[iQ][1]==1){
        brickLimit[iQ].iYbegin = NNY-1;
        brickLimit[iQ].iYend = NNY;
    }else if(lattice::Qvector[iQ][1]==0){
        brickLimit[iQ].iYbegin=BBegin;
        brickLimit[iQ].iYend=NNY;
    }else if(lattice::Qvector[iQ][1]==-1){
        brickLimit[iQ].iYbegin=BBegin;
        brickLimit[iQ].iYend=BBegin+1;
    }

    if (rank==0){

    //	cout<<brickLimit[iQ].iXbegin<<"  "<<brickLimit[iQ].iXend<<"  "<<brickLimit[iQ].iYbegin<<"  "<<brickLimit[iQ].iYend<<endl;

    }
}
}
// setting Mortar Layer limits for D2Q9 - check for loop limits of 0 if it runs!
BoxLimit mortarLimit[lattice::nQ];
int NNX=brick.getNX();
int NNY=brick.getNY();
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

    if (rank==0){

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
int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
if (iQ==2 or iQ==4){
    stride = brick.getNY();
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
    int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
    for (int iBuffer=0;iBuffer<length;++iBuffer){
        Distro distro;
        boundarySendBuffer[iQ].push_back(distro);
        boundaryRecvBuffer[iQ].push_back(distro);
    }
}

//    for (int iX=brickLimit[iBoundary].iXbegin;iX<brickLimit[iBoundary].iXend;++iX){
//        for (int iY=brickLimit[iBoundary].iYbegin;iY<brickLimit[iBoundary].iYend;++iY){
//    int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
//    for (int iBuffer=0;iBuffer<length;++iBuffer){



// Send Boundary Buffers
/*
MPI_Request reqs,reqr;
MPI_Status status;
//for (int iQ=1;iQ<lattice::nQ;++iQ)
if (rank==0)
{
    int iQ = 2;
    int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
// printing
    for(int ib=0;ib<length;++ib)
    {
        for (int i=0;i<lattice::nQ;++i)
        cout<<"rank="<<rank<<"  node ID="<<ib<<"  dir="<<i<<"  f= "<<boundarySendBuffer[iQ][ib].getF(i)<<endl;
    }
    cout<<"-----------------------"<<endl;
// printing finished
    MPI_Isend(&boundarySendBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,brick.getMPIneighbor(iQ), iQ, MPI_COMM_WORLD,&reqs);
}
//for (int iQ=1;iQ<lattice::nQ;++iQ)
if(rank==1)
{
    int iQ = 4;
    int iOp = lattice::iOpposite[iQ];
    int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
    MPI_Recv(&boundaryRecvBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,brick.getMPIneighbor(iOp), iOp, MPI_COMM_WORLD,&status);

// printing
    for(int ib=0;ib<length;++ib)
    {
        for (int i=0;i<lattice::nQ;++i)
        cout<<" Afterrank="<<rank<<"  node ID="<<ib<<"  dir="<<i<<"  f= "<<boundaryRecvBuffer[iQ][ib].getF(i)<<endl;
    }
    cout<<"-----------------------"<<endl;
// printing finished

}
*/

for (int t=0;t<10;++t){
// Copy domain boundaries to send buffer
for (int iQ=1;iQ<lattice::nQ;++iQ){
    int iBuffer =0;
    for (int iX=brickLimit[iQ].iXbegin;iX<brickLimit[iQ].iXend;++iX){
        for (int iY=brickLimit[iQ].iYbegin;iY<brickLimit[iQ].iYend;++iY){

            boundarySendBuffer[iQ][iBuffer] = brick(iX,iY);
            iBuffer++;
        }
    }
}

// Send & Recv
MPI_Request reqs,reqr;
MPI_Status status;
for (int iQ=1;iQ<lattice::nQ;++iQ)
{
    int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
    MPI_Isend(&boundarySendBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,brick.getMPIneighbor(iQ), iQ, MPI_COMM_WORLD,&reqs);
}
for (int iQ=1;iQ<lattice::nQ;++iQ)
{
    int iOp = lattice::iOpposite[iQ];
    int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
    MPI_Recv(&boundaryRecvBuffer[iQ][0].getF(0), length*lattice::nQ, MPI_DOUBLE,brick.getMPIneighbor(iQ), iOp, MPI_COMM_WORLD,&status);
}

// Copying receive buffer into Domain mortar nodes
for (int iQ=1;iQ<lattice::nQ;++iQ){
    int iBuffer =0;
    for (int iX=mortarLimit[iQ].iXbegin;iX<mortarLimit[iQ].iXend;++iX){
        for (int iY=mortarLimit[iQ].iYbegin;iY<mortarLimit[iQ].iYend;++iY){
            // TODO :: overload this loop
            for (int iQQ=0;iQQ<lattice::nQ;++iQQ){
                brick(iX,iY)[iQQ] = boundaryRecvBuffer[iQ][iBuffer].getF(iQQ);
            }

            iBuffer++;
        }
    }
}

brick.stream();
//brick.revStream(1,brick.getNX(),1,brick.getNY());
MPI_Barrier(MPI_COMM_WORLD);
// Printing
for (int iX=0;iX<brick.getNX();++iX){
    for (int iY=0;iY<brick.getNY();++iY){
        for (int iQ=0;iQ<lattice::nQ;++iQ)
        {
            double f = brick(iX,iY)[iQ];
            if (f>0.){
                cout<<" t="<<t<<" rank="<<rank<<"  node iX="<<iX<<" node iY="<<iY<<"  dir="<<iQ<<"  f= "<<f<<endl;
            }
        }
    }
}


// printing
for (int iQ=1;iQ<lattice::nQ;++iQ)
{
    int length = (brickLimit[iQ].iYend - brickLimit[iQ].iYbegin)*(brickLimit[iQ].iXend - brickLimit[iQ].iXbegin);
    for(int ib=0;ib<length;++ib)
    {
        for (int i=0;i<lattice::nQ;++i)
        {
            double f = boundaryRecvBuffer[iQ][ib].getF(i);
            if (f>0.){
              //  cout<<" rank="<<rank<<"  BC dir="<<iQ<<"  node ID="<<ib<<"  dir="<<i<<"  f= "<<f<<endl;
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

