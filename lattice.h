#ifndef LATTICE_HH
#define LATTICE_HH
// Choosing the lattice
#define lattice D2Q9

#include<vector>
#include<iostream>

namespace D2Q9 {

// number of streaming vectors (Q-vectors)
const int nQ = 9;

// Lattice dimensions
const int nD = 2;

// Squared of micro-velocity (c)
const double cSqr = 1.;
const double cQad = 1.;

// Lattice vectors, which are usually shown by "ei" in the literature.
// In SIMPLB, for the sake of being easily identified, they are called Qvectors.
// With the indexes of iQ =     0      1       2        3      4
const int Qvector[nQ][nD]  = {{0,0}, {1,0},  {0,1},  {-1,0}, {0,-1}, //                              5       6       7       8
                              {1,1}, {-1,1}, {-1,-1}, {1,-1}};

// Index of opposite vectors of {0, 1, 2, 3, 4, 5, 6, 7, 8}
const int iOpposite[nQ] =       {0, 3, 4, 1, 2, 7, 8, 5, 6};

// Index of half of the lattice vectors
const int iHalfQs[nQ/2] = {1,2,5,6};

// Weight of lattice vectors
const double weight[nQ] = {(double) 4./9.,
		(double) 1./9., (double) 1./9.,(double) 1./9.,(double) 1./9.,
		(double) 1./36., (double) 1./36., (double) 1./36., (double) 1./36.};
}
#endif
