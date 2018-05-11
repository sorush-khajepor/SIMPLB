#include"lattice.h"

class Node;

class collisionBGK{
    double omega;
public:    
    virtual void collide(Node& node);
    virtual double getFeq(const int& iQ, const double& rho, const double& uSqr,double u[lattice::nD]);
    virtual double getRho(const Node& node);
    virtual void calcU(const Node&, const double& rho, double u[]);
    virtual void calcRho_U(const Node& node,double& rho, double u[]);
    virtual void calc1stMoment(const Node&, double moment[]);
    virtual double getUsqr(const double u[]);
};
