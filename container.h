#include "lattice.h" 
#include<iostream>


// A class containing only data array with the length of number of lattice velocities (nQ)
template <class T>
class arrayNQ{
        T data[lattice::nQ];
public:
        arrayNQ(){
            *this=0.;
        }
        arrayNQ(const T rhs[]){
            *this=rhs;
        }

        arrayNQ& operator= (const T& rhs){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs;
                }
            }
        arrayNQ& operator= (const T rhs[]){
                for (int iQ=0;iQ<lattice::nQ;iQ++){
                    data[iQ] = rhs[iQ];
                }
            }
        arrayNQ& operator= (const arrayNQ& rhs){
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


// An array containing only cartesian data.
template <class T>
class arrayND{
        T data[lattice::nD];
public:
        // Initialized with zero
        arrayND(){
            *this=0.;
        }
        
        // Initialized with a standard C array
        arrayND(const T rhs[]){
            *this=rhs;
        }

        // Filling array with one value 
        arrayND& operator= (const T& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs;
                }
            }
        // Filling array with another arrayND
        arrayND& operator= (const arrayND& rhs){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs[iD];
                }
            }
        // Filling array with another standard c array
        arrayND& operator= (const T rhs[]){
                for (int iD=0;iD<lattice::nD;iD++){
                    data[iD] = rhs[iD];
                }
            }
        // An array member is called by its index
        T& operator[](const int& iD){
            return data[iD];
        }
        
        // Const call to an array member
        const T& operator[] (const int& iD)const{
            return data[iD];
        }
        
        // Print array on screen
        void print () {
            for (int iD=0;iD<lattice::nD;iD++){
                std::cout<< "data("<<iD<<") = "<<data[iD]<< std::endl;
            }
        }
};
