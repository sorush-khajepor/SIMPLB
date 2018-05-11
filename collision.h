#include"lattice.h"

class Node;

class collisionBGK{
    double tau;
public:    
    virtual void collide(){};

    virtual double getFeq(const int& iQ, const double& rho, const double& uSqr,double u[lattice::nD]);
    virtual double getRho(const Node& node);
    
    

    virtual double* getU(){double* k;return k;};
    virtual double* getFirstMoment(){double* j; return j;};
};
