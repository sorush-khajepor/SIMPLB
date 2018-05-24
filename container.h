#include "lattice.h" 
#include<iostream>


// A class containing only data vector with the length of lattice vectors.
// double lattice vector(dlv)
template <class T>
class dlv{
        T data[lattice::nQ];
public:
        dlv(){
            *this=0.;
        }

        dlv& operator= (const T& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs;
                }
            }
        dlv& operator= (const dlv& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs[iQ];
                }
            }
        T& operator[](const int& iQ){
            return data[iQ];
        }
        const T& operator[] (const int& iQ)const{
            return data[iQ];
        }

        void print () {
            for (int iQ=0;iQ<lattice::nQ;iQ++){
                std::cout<< "data("<<iQ<<") = "<<data[iQ]<< std::endl;
            }
        }
};

// A class containing only integer data type vector with the length of lattice vectors.
// integer lattice vector(ilv)
class ilv{
        int data[lattice::nQ];
public:
        ilv(){
            *this=0.;
        }

        ilv& operator= (const int& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs;
                }
            }
        ilv& operator= (const ilv& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs[iQ];
                }
            }
        int& operator[](const int& iQ){
            return data[iQ];
        }
        const int& operator[] (const int& iQ)const{
            return data[iQ];
        }
        void print () {
            for (int iQ=0;iQ<lattice::nQ;iQ++){
                std::cout<< "data("<<iQ<<") = "<<data[iQ]<< std::endl;
            }
        }
};

// A class containing only a cartesian vector.
// Double Cartesian Vector(dcv)

class dcv{
        double data[lattice::nD];
public:
        dcv(){
            *this=0.;
        }

        dcv& operator= (const double& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs;
                }
            }
        dcv& operator= (const dcv& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs[iD];
                }
            }
        double& operator[](const int& iD){
            return data[iD];
        }
        const double& operator[] (const int& iD)const{
            return data[iD];
        }

        void print () {
            for (int iD=0;iD<lattice::nD;iD++){
                std::cout<< "data("<<iD<<") = "<<data[iD]<< std::endl;
            }
        }
};

// A class containing only integer data type vector with the length of lattice vectors.
// integer cartesian vector(icv)
class icv{
        int data[lattice::nD];
public:
        icv(){
            *this=0.;
        }

        icv& operator= (const int& rhs){
            for (int iD=0;iD<lattice::nD;iD++){
                data[iD] = rhs;
            }
        }
        icv& operator= (const icv& rhs){
            for (int iD=0;iD<lattice::nD;iD++){
                data[iD] = rhs[iD];
            }
        }
        int& operator[](const int& iD){
            return data[iD];
        }
        const int& operator[] (const int& iD)const{
            return data[iD];
        }
        void print () {
            for (int iD=0;iD<lattice::nD;iD++){
                std::cout<< "data("<<iD<<") = "<<data[iD]<< std::endl;
            }
        }
};
