#ifndef LATTICE_H
#define LATTICE_H

// Choosing the lattice
#define lattice D2Q9

#include<iostream>
#include "sArray.h"

// D2Q9 lattice namespace
namespace D2Q9 {

// Number of streaming vectors (Q-vectors)
const int nQ = 9;

// Lattice dimensions
const int nD = 2;

// Squared of micro-velocity (c)
const double cSqr = 1.;
const double cQad = 1.;

// Lattice vectors, which are usually shown by "ei" in the literature.
// In SIMPLB, for the sake of being easily identified, they are called Qvectors.
// With the indexes of iQ =                      0        1        2        3        4
const  SArray<SArray<int,nD>,nQ> Qvector =  {{ 0, 0}, { 1, 0}, { 0, 1}, {-1, 0}, { 0,-1},

//                                              5        6        7        8
                                             { 1, 1}, {-1, 1}, {-1,-1}, { 1,-1}};

/* D2Q9 lattice configuration
*
*      6  2  5
*      3  0  1
*      7  4  8
*/

// Index of opposite vectors of {0, 1, 2, 3, 4, 5, 6, 7, 8}
const SArray<int,nQ> iOpposite = {0, 3, 4, 1, 2, 7, 8, 5, 6};

// Index of half of the lattice vectors
const SArray<int,nQ/2> iHalfQs = {1,2,5,6};

// Weight of lattice vectors            0
const SArray<double,nQ> weight = { 4./9.,
//                                      1       2       3       4
                                	   1./9.,  1./9.,  1./9.,  1./9.,
//                                      5       6       7       8   -
                                	   1./36., 1./36., 1./36., 1./36.};
}

typedef SArray<double,lattice::nD> doubleND;
typedef SArray<int,lattice::nD> intND;
typedef SArray<double,lattice::nQ> doubleNQ;
typedef SArray<int,lattice::nQ> intNQ;

#endif
