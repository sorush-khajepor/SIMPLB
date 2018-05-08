#include"node.h"

class Domain {

protected:
    int nX, nY, size;

	Node *nodes;

public:

	Domain(int nX_, int nY_):nX(nX_),nY(nY_),size(nX*nY){
		
	nodes = new Node[nX_*nY_];
		
	}

	Node& operator() (const int& iX, const int& iY){
			return nodes[getIndex(iX,iY)];	
		}

	int getIndex(const int& iX, const int& iY){return iX*nY+iY;}
		

	Node& operator[] (const int& iNode){return nodes[iNode];}

    int getNX(){return nX;}	
    int getNY(){return nY;}	
    int getSize() {return size;}	

	void print() {
		cout<< "nX = " << nX<<endl;
		cout<< "nY = " << nY<<endl;
		cout<< "size  = " << size<<endl;
	}
	
	void getPeriodicNeighbor (const int& iX, const int& iY, const int& iQ, int& iX_neighbor, int& iY_neighbor) {
	    iX_neighbor = (iX + Qvectors[iQ][0]+nX)%nX;
	    iY_neighbor = (iY + Qvectors[iQ][1]+nY)%nY;
	}


	void revStream(){
		int id, id_neighbor;
	    int iX_neighbor,iY_neighbor;
	    int q;
		
        for (int iX=0;iX<nX;++iX){
            for (int iY=0;iY<nY;++iY){
            
            	id = getIndex(iX,iY);
            	for (vector<int>::const_iterator iQ = halfQs.begin();iQ!=halfQs.end();++iQ){
            		
        			q = *iQ;
        			getPeriodicNeighbor(iX,iY,q,iX_neighbor,iY_neighbor);
                    id_neighbor = getIndex(iX_neighbor,iY_neighbor); 
        			std::swap(nodes[id][q],nodes[id_neighbor][opposite[q]]);
            	}
            }
        }
	}
	
	void mirror(){
		for (int iNode=0;iNode<size;++iNode){
			nodes[iNode].mirror();
		}
	}

};