#include"node.h"
#include"geometryTools.h"

class Block {

protected:

    int origin[lattice::nD];
    int dim[lattice::nD];
    // Volume of the block (surface area in 2D)
    int vol;

    Node *nodes;

    // This stores neighbor blocks (or MPI processes) in the directions of Qvector in the MPI world.
    // Note, each Block is assigned to an MPI process.
    int MPIneighbor[lattice::nQ];

    LoopLimit boundaryLimit[lattice::nQ];

    LoopLimit ghostLimit[lattice::nQ];

public:

    Block(int origin_[], int dim_[]){
	// Initialization of geometry
	vol =1;
       for (int iD=0;iD<lattice::nD;++iD){
	       origin[iD]=origin_[iD];
	       dim[iD]=dim_[iD];
	       vol*=dim_[iD];
       }
		
       // Memory allocation for nodes
        nodes = new Node[vol];

	//  Set loop limits for actual and ghost  boundaries 
        setBoundaryLimit();
        setGhostLimit();
		
    }

    Node& operator() (const int& iX, const int& iY){
        return nodes[getIndex(iX,iY)];
    }

	Node const& operator() (const int& iX, const int& iY) const {
        return nodes[getIndex(iX,iY)];
    }

    int getIndex (const int& iX, const int& iY) const {return iX*nY+iY;}
		

    Node& operator[] (const int& iNode){return nodes[iNode];}

    int getNX() const {return geo.getDim(0);}
    int getNY() const {return nY;}
    int getSize() const {return size;}

    // Gives the neighbor block (or MPI process) in the direction of iQ.
    int& getMPIneighbor (const int& iQ){return MPIneighbor[iQ];}

    // Prints class members
    void print() {
        cout<< "nX = " << nX<<endl;
        cout<< "nY = " << nY<<endl;
        cout<< "size  = " << size<<endl;
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            cout<<"iQ="<<iQ<<"boundaryLimit iXbegin="<<boundaryLimit[iQ].getBegin(0)<<" iXend="<<boundaryLimit[iQ].getEnd(0)
                <<"  iYbegin="<<boundaryLimit[iQ].getBegin(1)<<" iYend="<<boundaryLimit[iQ].getEnd(1)<<endl;
        }
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            cout<<"iQ="<<iQ<<"ghostLimit iXbegin="<<ghostLimit[iQ].getBegin(0)<<" iXend="<<ghostLimit[iQ].getEnd(0)
                <<"  iYbegin="<<ghostLimit[iQ].getBegin(1)<<" iYend="<<ghostLimit[iQ].getEnd(1)<<endl;
        }
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            cout<<"iQ="<<iQ<<"MPI neighbor="<<MPIneighbor[iQ];
        }
    }

    // Finds the neighbor of the node (iX,iY) in the direction of iQ for a periodic block.
    void getPeriodicNeighbor (const int& iX, const int& iY, const int& iQ, int& iX_neighbor, int& iY_neighbor) {
        iX_neighbor = (iX + lattice::Qvector[iQ][0]+geo.getDim(0))%geo.getDim(0);
        iY_neighbor = (iY + lattice::Qvector[iQ][1]+nY)%nY;
    }

    // Stream in reversed direction over the whole block. The mirror() function, after this can
    // correct them.
    void revStream(){
        int id, id_neighbor;
        int iX_neighbor,iY_neighbor;
        int iQ;
		
        for (int iX=0;iX<geo.getDim(0);++iX){
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
	
    // Mirroring distros all over the block.
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

    // Setting the limits of boundaries in shape of :
    // iXbegin->iXend
    // iYbegin->iYend
    // for edges and corners in D2Q9 lattice.
    // Note, Ghost nodes are excluded.
    // Note, block boundaries are identified by iQ.
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

    // Setting the limits of Ghost nodes boundaries in shape of :
    // iXbegin->iXend
    // iYbegin->iYend
    // for edges and corners in D2Q9 lattice.
    // Note, boundaries are identified by iQ.
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