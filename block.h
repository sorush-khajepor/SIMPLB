#ifndef BLOCK_H
#define BLOCK_H

#include"node.h"
#include"geometryTools.h"
#include "domainDecomposition.h"
#include <vector>



class Message;

class Block {

protected:

    // Cartesian index of the block
    intND cartIndex;

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

    // Limits of loops for covering block ghosts
    LoopLimitNQ ghostLimit;

    // Limits of loops for covering block extended boundaries
    LoopLimitNQ extendedBoundaryLimit;

    // Limits of loops for covering block boundaries
    LoopLimitNQ boundaryLimit;

    // Limits of loops for covering block boundaries
    LoopLimitNQ innerLayerLimit;

    // Limits of loops for covering block boundaries
    LoopLimit coreLimit;


public:

    // Block construction
    Block(const StructuredDecomposition& sd);

    // Gets message member
    Message& getMessage(){return *message;}

    // Give access to boundary&Ghost Limit members
    LoopLimitNQ& getExtendedBoundaryLimit(){return extendedBoundaryLimit;}
    LoopLimitNQ& getGhostLimit(){return ghostLimit;}
    LoopLimitNQ& getBoundaryLimit(){return boundaryLimit;}
    LoopLimitNQ& getInnerLayerLimit(){return innerLayerLimit;}
    LoopLimit&   getCoreLimit(){return coreLimit;}

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

    void revStream(const LoopLimit& ll){

        for (int iX=ll.getBegin()[0];iX<ll.getEnd()[0];++iX){
            for (int iY=ll.getBegin()[1];iY<ll.getEnd()[1];++iY){
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

    void revStream(const LoopLimit& ll, vector<int> iQlist){

            for (int iX=ll.getBegin()[0];iX<ll.getEnd()[0];++iX){
                for (int iY=ll.getBegin()[1];iY<ll.getEnd()[1];++iY){
                    int id = getShortIndex(iX,iY);
                    for (auto i=iQlist.begin();i!=iQlist.end();i++){

                        int iQ = *i;
                        int iX_neighbor,iY_neighbor;
                        getPeriodicNeighbor(iX,iY,iQ,iX_neighbor,iY_neighbor);
                        int id_neighbor = getShortIndex(iX_neighbor,iY_neighbor);
                        std::swap(nodes[id][iQ],nodes[id_neighbor][lattice::iOpposite[iQ]]);
                    }
                }
            }
        }
    //Reverse stream for boundaries (start from iQ=1).
    void revStream(const LoopLimitNQ& ll){

        for (int iQ=1;iQ<lattice::nQ;iQ++){
            revStream(ll[iQ]);
        }
    }

    void revStream(const LoopLimitNQ& ll, const SArrayBase<std::vector<int>,lattice::nQ> iQlist){

        for (int iQ=1;iQ<lattice::nQ;iQ++){
            revStream(ll[iQ], iQlist[iQ]);
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
    void setExtendedBoundaryLimit(){

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

            extendedBoundaryLimit[iQ]={{iXbegin,iYbegin},{iXend,iYend}};
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

            ghostLimit[iQ]={{iXbegin,iYbegin},{iXend,iYend}};

        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Block& block){
        os<<"Cartesian Index = "<<block.cartIndex;
        os<<"Origin = "<<block.origin;
        os<<"Dimensions = "<<block.dim;
        os<<"Volume = "<<block.vol<<endl;
    //    os<<"extendedBoundaryLimit = "<<block.extendedBoundaryLimit;
//        os<<"ghostLimit = "<< block.ghostLimit;
        os<<std::endl<<std::endl;
        return os;
    }
};
#endif
