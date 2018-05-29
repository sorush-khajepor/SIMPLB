#include"node.h"
#include"geometryTools.h"

typedef SArray<LoopLimit,lattice::nQ> LoopLimitNQ;

class Message;

class Block {

protected:

    // Origin of the block (including the ghost layers)
    intND origin;

    // Dimensions of the block (including the ghost layers)
    intND dim;

    // Volume of the block (surface area in 2D)
    int vol;

    // Nodes in this block
    Node *nodes;

    // Communication with neighboring blocks
    Message *message;

    // Limits of loops for covering block boundaries (ignoring ghosts)
    LoopLimitNQ boundaryLimit;

    // Limits of loops for covering block boundaries (including ghosts)
    LoopLimitNQ ghostLimit;

public:

    // Block construction
    Block(const intND& origin_, const intND& dim_){

        // Initialization of geometry
	       origin=origin_;
	       dim=dim_;
	       vol=dim_.computeVol();
        }

        // Memory allocation for nodes
        nodes = new Node[vol];

	    // Set loop limits for actual and ghost  boundaries
        setBoundaryLimit();
        setGhostLimit();

        //TODO Set message
        //message = new Message (boundaryLimit,ghostLimit,neighbor);


    }

    // Give access to boundary&Ghost Limit members
    LoopLimitNQ& getBoundaryLimit(){return boundaryLimit;}
    LoopLimitNQ& getGhostLimit(){return ghostLimit;}

    // Operator () is reserved for accessing nodes
    Node& operator() (const int& iX, const int& iY){
        return nodes[getShortIndex(iX,iY)];
    }

    // Constant access to Node through operator ()
	const Node& operator() (const int& iX, const int& iY) const {
        return nodes[getShortIndex(iX,iY)];
    }

	// Get single number index of the node from cartesian index
    int getShortIndex (const int& iX, const int& iY) const {return iX*dim[1]+iY;}

    // Get Cartesian index (iX,iY,iZ) from single index
    int getLongIndex (const int& iNode, int& iX, int& iY) const {iX=iNode/dim[1];iY=iNode%dim[1];}

    // Operator [] is resereved for accessing  a node via single index
    Node& operator[] (const int& iNode){return nodes[iNode];}

    // Give access to block geometry
    const int& getDim(const int& iD) const {return dim[iD];}
    const int& getOrigin(const int& iD) const {return origin[iD];}
    const int& getVol() const {return vol;}

    // Prints class members
    void print() {
        cout<< "DimX = " << getDim(0)<<endl;
        cout<< "DimY = " << getDim(1)<<endl;
        cout<< "Volume  = " << vol<<endl;
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            cout<<"iQ="<<iQ<<"boundaryLimit iXbegin="<<boundaryLimit[iQ].getBegin(0)<<" iXend="<<boundaryLimit[iQ].getEnd(0)
                <<"  iYbegin="<<boundaryLimit[iQ].getBegin(1)<<" iYend="<<boundaryLimit[iQ].getEnd(1)<<endl;
        }
        for (int iQ=0;iQ<lattice::nQ;++iQ){
            cout<<"iQ="<<iQ<<"ghostLimit iXbegin="<<ghostLimit[iQ].getBegin(0)<<" iXend="<<ghostLimit[iQ].getEnd(0)
                <<"  iYbegin="<<ghostLimit[iQ].getBegin(1)<<" iYend="<<ghostLimit[iQ].getEnd(1)<<endl;
        }
    }

    // Finds the neighbor of the node (iX,iY) in the direction of iQ for a periodic block.
    void getPeriodicNeighbor (const int& iX, const int& iY, const int& iQ, int& iX_neighbor, int& iY_neighbor) {
        iX_neighbor = (iX + lattice::Qvector[iQ][0]+dim[0])%dim[0];
        iY_neighbor = (iY + lattice::Qvector[iQ][1]+dim[1])%dim[1];
    }

    // Stream in reversed direction over the whole block. The mirror() function, after this can
    // correct them.
    void revStream(){

        for (int iX=0;iX<dim[0];++iX){
            for (int iY=0;iY<dim[1];++iY){
                int id = getShortIndex(iX,iY);
                for (int i=0;i<lattice::nQ/2;++i){

                    int iQ = lattice::iHalfQs[i];
                    int iX_neighbor,iY_neighbor;
                    getPeriodicNeighbor(iX,iY,iQ,iX_neighbor,iY_neighbor);
                    int id_neighbor = getShortIndex(iX_neighbor,iY_neighbor);
                    std::swap(nodes[id][iQ],nodes[id_neighbor][lattice::iOpposite[iQ]]);
                }
            }
        }
    }

    // Mirroring distros all over the block.
    void mirror(){
        for (int iNode=0;iNode<vol;++iNode){
            nodes[iNode].mirror();
        }
    }

    // Domain stream
    void stream(){
        revStream();
        mirror();
    }

    // Replace the data of this block with another block (rhs)
    Block& operator= (const double& rhs){

        for (int iNode=0;iNode<vol;++iNode){
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
                iXbegin = dim[0]-2;
                iXend = dim[0]-1;
            }else if(lattice::Qvector[iQ][0]==0){
                iXbegin=1;
                iXend=dim[0]-1;
            }else if(lattice::Qvector[iQ][0]==-1){
                iXbegin=1;
                iXend=2;
            }

            if (lattice::Qvector[iQ][1]==1){
                iYbegin = dim[1]-2;
                iYend = dim[1]-1;
            }else if(lattice::Qvector[iQ][1]==0){
                iYbegin=1;
                iYend=dim[1]-1;
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
                iXbegin = dim[0]-1;
                iXend = dim[0];
            }else if(lattice::Qvector[iQ][0]==0){
                iXbegin=1;
                iXend=dim[0]-1;
            }else if(lattice::Qvector[iQ][0]==-1){
                iXbegin=0;
                iXend=1;
            }

            if (lattice::Qvector[iQ][1]==1){
                iYbegin = dim[1]-1;
                iYend = dim[1];
            }else if(lattice::Qvector[iQ][1]==0){
                iYbegin=1;
                iYend=dim[1]-1;
            }else if(lattice::Qvector[iQ][1]==-1){
                iYbegin=0;
                iYend=1;
            }

            ghostLimit[iQ].set(iXbegin,iYbegin,iXend,iYend);

        }
    }
};
