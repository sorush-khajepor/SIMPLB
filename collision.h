#include"lattice.h"

class Node;

//////////// BGK Collision /////////////

class collisionBGK{
	// Destructor
	//virtual ~collisionBGK() {}
	// Relaxation parameter
	double omega;
public:
	collisionBGK(const double& omega_){omega=omega_;};
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

//////////// Standard Bounce-Back /////////////

class bounceBack : public collisionBGK{
	// Relaxation parameter
	double omega;
public:
	// Collision step
	virtual void collide(Node& node) override;
};

//////////// Zou-He Boundary Condition /////////////

class ZouHeCollisionBottom : public collisionBGK{
	// Relaxation parameter
	double omega;
	// External Force
	double force[lattice::nD];
	// Fluid velocity
	double u[lattice::nD];
public:
	// Collision step
	virtual void collide(Node& node) override;
    // Fluid density
    virtual double getRho(const Node& node) override;
    // Get missing Distribution functions
    virtual void getMissingDistros(Node& node, double& Rho);
};
