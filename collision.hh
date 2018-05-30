#include "collision.h"
#include "node.h"
#include "geometryTools.h"

//////////// BGK Collision /////////////

collisionBGK::collisionBGK(const double& omega_){
	omega = omega_;
};

void collisionBGK::collide(Node& node){
    double Rho,Usqr;
    double U[lattice::nD];
    calcRho_U(node,Rho,U);
    Usqr = getUsqr(U);
    for(int iQ=0;iQ<lattice::nQ;++iQ){
        node[iQ] = (1. - omega)*node[iQ] + omega*getFeq(iQ,Rho,Usqr,U);
    }
}


double collisionBGK::getFeq(const int& iQ, const double& Rho, const double& Usqr,double U[lattice::nD]){
    double Uq = 0.;
    for (int iD=0;iD<lattice::nD;++iD){
       Uq += U[iD]*lattice::Qvector[iQ][iD];
    }
    
    return  lattice::weight[iQ] * Rho * (1. + 3. * Uq/lattice::cSqr + 4.5 * Uq * Uq/lattice::cQad
                                - 1.5 * Usqr / lattice::cSqr);
}
    
double collisionBGK::getRho(const Node& node){
    double Rho = 0.;
    for (int iQ=0;iQ<lattice::nQ;++iQ){
       Rho += node[iQ];
    }
    return Rho;
}

double collisionBGK::getUsqr(const double U[]){
    double Usqr=0.;
    for(int iD=0;iD<lattice::nD;++iD){
        Usqr += U[iD]*U[iD];
    }
    return Usqr;
}

void collisionBGK::calcU(const Node& node,const double& Rho, double U[]){
    for (int iD = 0;iD<lattice::nD;++iD){
        U[iD] = 0.;
        for (int iQ=0;iQ<lattice::nQ;++iD){
            U[iD] += lattice::Qvector[iQ][iD]*node[iQ];
        }
        U[iD]/=Rho;
    }
}

void collisionBGK::calcRho_U(const Node& node,double& Rho, double U[]){
    Rho = getRho(node);
    calcU(node,Rho,U);
}


void collisionBGK::calc1stMoment(const Node& node, double moment[]){
    for (int iD = 0;iD<lattice::nD;++iD){
        moment[iD] = 0.;
        for (int iQ=0;iQ<lattice::nQ;++iD){
            moment[iD] += lattice::Qvector[iQ][iD]*node[iQ];
        }
    }
}

//////////// BGK Collision with Forcing /////////////

collisionForcedBGK::collisionForcedBGK(const double& omega_, double force){
	omega = omega_;
};

void collisionForcedBGK::collide(Node& node){
    double Rho,Usqr;
    double U[lattice::nD];
    calcRho_U(node,Rho,U);
    Usqr = getUsqr(U);
    for(int iQ=0;iQ<lattice::nQ;++iQ){
        node[iQ] = (1. - omega)*node[iQ] + omega*getFeq(iQ,Rho,Usqr,U);
    }
}

void collisionForcedBGK::calcU(const Node& node,const double& Rho, double U[]){
    for (int iD = 0;iD<lattice::nD;++iD){
        U[iD] = 0.;
        for (int iQ=0;iQ<lattice::nQ;++iD){
            U[iD] += (lattice::Qvector[iQ][iD]*node[iQ]) + 0.5*force;
        }
        U[iD]/=Rho;
    }
}

void collisionForcedBGK::calcRho_U(const Node& node,double& Rho, double U[]){
    Rho = getRho(node);
    calcU(node,Rho,U);
}

//////////// Standard Bounce-Back /////////////

bounceBack::bounceBack(const double& omega_){
	omega = omega_;
};

void bounceBack::collide(Node& node){
	node.mirror();
}

//////////// Zou-He Boundary Conditions /////////////

//////////// Top /////////////

ZouHeCollisionTop::ZouHeCollisionTop(const double& omega_, double& force, double& u){
	omega = omega_;
};

void ZouHeCollisionTop::collide(Node& node){
    double Usqr;
	double Rho;
    ZouHeCollisionTop::getMissingDistros(node,Rho);
    calcRho_U(node,Rho,u);
    Usqr = getUsqr(u);
    for(int iQ=0;iQ<lattice::nQ;++iQ){
        node[iQ] = (1. - omega)*node[iQ] + omega*getFeq(iQ,Rho,Usqr,u);
    }
}

double ZouHeCollisionTop::getZHRho(const Node& node){
    double Rho = 0.;
    int norm[2] = {0,1};
    Rho = (2.*(Node[0]+Node[1]+Node[3])+4.*(Node[2]+Node[5]+Node[6])+(getDotProduct(norm,force)))/2.*(1.+(getDotProduct(norm,u)));
    }
    return Rho;
}

void ZouHeCollisionTop::getMissingDistros(Node& node, double& Rho){
	Rho = getZHRho(node);
	Node[4] = (Node[2]-2.*(Rho*u[1])/3.);
	Node[7] = (Node[5]-(Rho*u[1])/6.-(Rho*u[0])/2.+(Node[3]-Node[1])/2.-(-force[0]-force[1])/4.); //TODO check equ's
	Node[8] = (Node[6]-(Rho*u[1])/6.+(Rho*u[0])/2.+(Node[1]-Node[3])/2.-(force[0]-force[1])/4.);
}

//////////// Bottom /////////////

ZouHeCollisionBottom::ZouHeCollisionBottom(const double& omega_, double& force, double& u){
	omega = omega_;
};

void ZouHeCollisionBottom::collide(Node& node){
	double Usqr;
	double Rho;
	ZouHeCollisionBottom::getMissingDistros(node,Rho);
	calcRho_U(node,Rho,u);
	Usqr = getUsqr(u);
	for(int iQ=0;iQ<lattice::nQ;++iQ){
	    node[iQ] = (1. - omega)*node[iQ] + omega*getFeq(iQ,Rho,Usqr,u);
	}
}

double ZouHeCollisionBottom::getZHRho(const Node& node){
    double Rho = 0.;
    int norm[2] = {0,-1};
    Rho = (2.*(Node[0]+Node[1]+Node[3])+4.*(Node[4]+Node[7]+Node[8])+(getDotProduct(norm,force)))/2.*(1.+(getDotProduct(norm,u)));
    }
    return Rho;
}

void ZouHeCollisionBottom::getMissingDistros(Node& node, double& Rho){
	Rho = getRho(node);
	Node[2] = (Node[4]+2.*(Rho*u[1])/3.);
	Node[5] = (Node[7]+(Rho*u[1])/6.+(Rho*u[0])/2.+(Node[3]-Node[1])/2.-(force[0]+force[1])/4.);
	Node[6] = (Node[8]+(Rho*u[1])/6.+(Rho*u[0])/2.+(Node[1]-Node[3])/2.-(-force[0]+force[1])/4.);
}

