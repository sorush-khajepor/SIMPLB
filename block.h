#include"node.h"
#include"geometryTools.h"

class Block {

protected:
    int nX, nY, size;

    Node *nodes;

    int MPIneighbor[lattice::nQ];

    Box boundaryLimit[lattice::nQ];

    Box ghostLimit[lattice::nQ];

public:

    Block(int nX_, int nY_):nX(nX_),nY(nY_),size(nX*nY){
		
        nodes = new Node[nX_*nY_];
		
    }

    Node& operator() (const int& iX, const int& iY){
        return nodes[getIndex(iX,iY)];
    }

	Node const& operator() (const int& iX, const int& iY) const {
        return nodes[getIndex(iX,iY)];
    }

    int getIndex (const int& iX, const int& iY) const {return iX*nY+iY;}
		

    Node& operator[] (const int& iNode){return nodes[iNode];}

    int getNX() const {return nX;}
    int getNY() const {return nY;}
    int getSize() const {return size;}

    int& getMPIneighbor (const int& iNeighbor){return MPIneighbor[iNeighbor];}

    void print() {
        cout<< "nX = " << nX<<endl;
        cout<< "nY = " << nY<<endl;
        cout<< "size  = " << size<<endl;
        for (int iQ=0;iQ<lattice::nQ;++iQ){
        cout<<"boundaryLimit iXbegin="<<boundaryLimit[iQ].getBegin(0)<<" iXend="<<boundaryLimit[iQ].getEnd(0)
            <<"  iYbegin="<<boundaryLimit[iQ].getBegin(1)<<" iYend="<<boundaryLimit[iQ].getEnd(1)<<endl;
        }

        for (int iQ=0;iQ<lattice::nQ;++iQ){
        cout<<"ghostLimit iXbegin="<<ghostLimit[iQ].getBegin(0)<<" iXend="<<ghostLimit[iQ].getEnd(0)
            <<"  iYbegin="<<ghostLimit[iQ].getBegin(1)<<" iYend="<<ghostLimit[iQ].getEnd(1)<<endl;
        }

    }
	
    void getPeriodicNeighbor (const int& iX, const int& iY, const int& iQ, int& iX_neighbor, int& iY_neighbor) {
        iX_neighbor = (iX + lattice::Qvector[iQ][0]+nX)%nX;
        iY_neighbor = (iY + lattice::Qvector[iQ][1]+nY)%nY;
    }


    void revStream(){
        int id, id_neighbor;
        int iX_neighbor,iY_neighbor;
        int iQ;
		
        for (int iX=0;iX<nX;++iX){
            for (int iY=0;iY<nY;++iY){
                id = getIndex(iX,iY);
                for (int i=0;i<lattice::nQ/2;++i){

                    iQ = lattice::iHalfQs[i];
                    getPeriodicNeighbor(iX,iY,iQ,iX_neighbor,iY_neighbor);
                    id_neighbor = getIndex(iX_neighbor,iY_neighbor); 
                    std::swap(nodes[id][iQ],nodes[id_neighbor][lattice::iOpposite[iQ]]);
                }
            }
        }
    }
	
    void mirror(){
        for (int iNode=0;iNode<size;++iNode){
            nodes[iNode].mirror();
        }
    }

    void stream(){
        revStream();
        mirror();
    }

    Block& operator= (const double& rhs){

        for (int iNode=0;iNode<size;++iNode){
            nodes[iNode]= rhs;
        }
    }

    void setBoundaryLimit(){

        int iXend,iYend,iXbegin,iYbegin;

        for (int iQ=0;iQ<lattice::nQ;++iQ){
            if (lattice::Qvector[iQ][0]==1){
                iXbegin = nX-2;
                iXend = nX-1;
            }else if(lattice::Qvector[iQ][0]==0){
                iXbegin=1;
                iXend=nX-1;
            }else if(lattice::Qvector[iQ][0]==-1){
                iXbegin=1;
                iXend=2;
            }

            if (lattice::Qvector[iQ][1]==1){
                iYbegin = nY-2;
                iYend = nY-1;
            }else if(lattice::Qvector[iQ][1]==0){
                iYbegin=1;
                iYend=nY-1;
            }else if(lattice::Qvector[iQ][1]==-1){
                iYbegin=1;
                iYend=2;
            }

            boundaryLimit[iQ].set(iXbegin,iYbegin,iXend,iYend);
        }
    }

    // setting Ghost Layer limits for D2Q9
    void setGhostLimit(){
        int iXbegin,iYbegin,iXend,iYend;
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            if (lattice::Qvector[iQ][0]==1){
                iXbegin = nX-1;
                iXend = nX;
            }else if(lattice::Qvector[iQ][0]==0){
                iXbegin=1;
                iXend=nX-1;
            }else if(lattice::Qvector[iQ][0]==-1){
                iXbegin=0;
                iXend=1;
            }

            if (lattice::Qvector[iQ][1]==1){
                iYbegin = nY-1;
                iYend = nY;
            }else if(lattice::Qvector[iQ][1]==0){
                iYbegin=1;
                iYend=nY-1;
            }else if(lattice::Qvector[iQ][1]==-1){
                iYbegin=0;
                iYend=1;
            }

            ghostLimit[iQ].set(iXbegin,iYbegin,iXend,iYend);

        }
    }
};
