#ifndef MPITOOLS_H
#define MPITOOLS_H

#include "mpi.h"
class MPItools{
    protected:
    int rank;
    int size;
    public:
    MPItools(){
            MPI_Init(NULL,NULL);
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &size);
    }

    int& getRank(){return rank;}
    int& getSize(){return size;}

};

MPItools mpiTools;

#endif
