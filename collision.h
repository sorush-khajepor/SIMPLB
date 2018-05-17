#include"lattice.h"

class Node;

//////////// BGK Collision /////////////

class collisionBGK{
    // Relaxation parameter
	double omega;
public:
	// Collision step
    virtual void collide(Node& node);
    // Equilibrium distribution function
    virtual double getFeq(const int& iQ, const double& rho, const double& uSqr,double u[lattice::nD]);
    // Fluid density
    virtual double getRho(const Node& node);
    // Calculate fluid velocity on node
    virtual void calcU(const Node&, const double& rho, double u[]);
    // Calculate fluid velocity and density on node
    virtual void calcRho_U(const Node& node,double& rho, double u[]);
    // Calculate moment
    virtual void calc1stMoment(const Node&, double moment[]);
    // Fluid velocity squared
    virtual double getUsqr(const double u[]);
};

//////////// BGK Collision with Forcing /////////////

class collisionForcedBGK : public collisionBGK{
	// Relaxation parameter
	double omega;
	// External force
	double force[lattice::nD];
public:
	// Collision step
	virtual void collide(Node& node) override;
	// Calculate fluid velocity on node
    virtual void calcU(const Node&, const double& rho, double u[]) override;
    // Calculate fluid velocity and density on node
    virtual void calcRho_U(const Node& node,double& rho, double u[]) override;
};
