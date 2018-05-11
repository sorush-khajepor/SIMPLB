#include "collision.h"
#include "node.h"


double getFeq(const int& iQ, const double& rho, const double& uSqr,double u[lattice::nD]){
    double Uq = 0.;
    for (int iD=0;iD<lattice::nD;++iD){
       Uq += u[iD]*lattice::Qvector[iQ][iD]; 
    }
    
    return  lattice::weight[iQ] * rho * (1. + 3. * Uq/lattice::cSqr + 4.5 * Uq * Uq/lattice::cQad
                                - 1.5 * uSqr / lattice::cSqr);
};
    
double collisionBGK::getRho(const Node& node){
    double rho = 0.;
    for (int iQ=0;iQ<lattice::nQ;++iQ){
       rho += node[iQ]; 
    }
    return rho;
};