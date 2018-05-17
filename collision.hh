#include "collision.h"
#include "node.h"

//////////// BGK Collision /////////////

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
